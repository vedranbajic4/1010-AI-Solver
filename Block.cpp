#include <bits/stdc++.h>
# define clr(x,a)        memset(x,a,sizeof(x))
# define all(a)          a.begin(), a.end()
# define deb(x)          cout<<#x<<"=="<<x<<endl;
# define deb2(x, y)      cout<<"("<<#x<<", "<<#y<<")==("<<x<<", "<<y<<")\n";
# define pb              push_back
# define sc              scanf
# define pr              printf
# define x               first
# define y               second
# define rev(s)          reverse(s.begin(),s.end())
# define linija          cout<<"___________\n";
# define FOR(i,j,k)      for(int i = j; i <= k; i++)
# define mp              make_pair

using namespace std;
typedef unsigned long long ll;
typedef vector<int> vi;
typedef pair<int, int> pii;

const int mxN = 27, mxM = 50000, mxDepth = 50;
const int mxBits = 11, N[4] = {3, 3, 3, 2};
const float koren2 = 1.41421356237;
const ll INF = 1e15;
const float FLOATINF = 1e8;
const int sr4[4] = {1, 0, -1, 0};
const int sc4[4] = {0, 1, 0, -1};
const int sr9[9] = {0, 0, 0, -1, -1, -1, 1, 1, 1};
const int sc9[9] = {-1, 1, 0, -1, 1, 0, -1, 1, 0};

int n, m, max_depth = 3, ntable, nframes, max_surf_const;
ll c[mxM], f[mxM];
clock_t END;
pii res[mxM];

ll board2[mxBits], temp, br;
ll moves[5][4][mxN][mxN][mxBits];
ll mask_rows[mxN][mxBits], mask_cols[mxN][mxBits];

ll frames[mxN*mxN][2][mxBits];

inline bool in_bounds(int& r, int& c){
   return (r >= 1 && r <= n && c >= 1 && c <= n);
}

inline bool resources_left(){
   return clock() <= END;
}

void cout_board(int d){
   cout << " Board (" << d << ") : ";
   FOR(i, 1, ntable){
      cout << board2[i] << " ";
   }
}

void cout_board(ll arr[]){
   bool bord[mxN][mxN];
   clr(bord, 0);
   int r = 1, c = 1, br;
   ll x;
   bool temp[70];

   FOR(i, 1, ntable){
      x = arr[i];
      br = 0;
      FOR(j, 0, 63){
         bord[r][c] = (x & (1LL<<j));
         c++;
         if(c > n){
            r++;
            c = 1;
         }
         if(r > n) break;
      }
   }
   cout << "Binarno:\n";
   FOR(i, 1, n){
      FOR(j, 1, n){
         cout << bord[i][j] << " ";
      }
      cout << endl;
   }
}

struct Heuristic{
   ///THIS CLASS IS resPONSIBILE FOR CALCULATING HEURISTICs
   int a, b, c, d, e, f, ret, square, equals;
   int A = 4, D = 8,  E = 6, F = -25;
// int A = 4, D = 8,  E = 6, F = -25;
   int surface_minimization(const ll (&board)[mxBits]){
      ret = 0;

      FOR(frame, 1, nframes){
         equals = 1;
         FOR(bit, 1, ntable){
            if( (frames[frame][0][bit] & board[bit]) != frames[frame][0][bit]){
               equals = 0;
               break;
            }
         }
         if(!equals) continue;

         FOR(bit, 1, ntable){
            ret += __builtin_popcountll( frames[frame][1][bit] & (~board[bit]) );
         }
      }
      return max_surf_const-ret;
   }

   int empty_cells(const ll (&board)[mxBits]){
      ret = 0;
      FOR(i, 1, ntable)
         ret += __builtin_popcountll(board[i]);
      return n*n-ret;
   }

   int alignment_maximization(const ll (&board)[mxBits]){
      ret = 0;
      FOR(i, 1, n){
         square = 0;
         FOR(j, 1, ntable){
            square += __builtin_popcountll((board[j] & mask_rows[i][j]));
         }
         ret += (n-square)*(n-square);
         square = 0;
         FOR(j, 1, ntable){
            square += __builtin_popcountll((board[j] & mask_cols[i][j]));
         }
         ret += (n-square)*(n-square);
      }
      ret /= (n*n);

      return ret;
   }

   int isolated_cells(const ll (&board)[mxBits]){
      ret = 0;

      FOR(frame, 1, nframes){
         equals = 1;
         FOR(bit, 1, ntable){
            if( (frames[frame][0][bit] & (~board[bit])) != frames[frame][0][bit]){
               equals = 0;
               break;
            }
         }
         if(equals){
            FOR(bit, 1, ntable){
               if( (frames[frame][1][bit] & board[bit]) != frames[frame][1][bit]){
                  equals = 0;
                  break;
               }
            }
            if(equals) ret++;
         }
      }

      return ret;
   }

   long long calc_heuristic(ll (&board)[mxBits]){
      a = empty_cells(board);

      d = alignment_maximization(board);

      e = surface_minimization(board);

      f = isolated_cells(board);

      return A*a + d*D + e*E + f*F;
   }
}h;

struct Binary{
   /// THIS CLASS IS resPONSIBLE FOR GENERATING AND PREPARING ALL BIT MASK MATRICES
   pii blocks[5][4][3];
   int nr, nc, cnt;
   bool temp_board[mxN][mxN];

   //initial call
   void init(){
      srand(588);
      generate_blocks();
      generate_binary_blocks();
      generate_masks();
      generate_frames();
   }

   //masks for rows and cols (|| or ___)
   void generate_masks(){
      FOR(i, 1, n){
         //rows
         clr(temp_board, 0);
         FOR(j, 1, n){
            temp_board[i][j] = 1;
         }
         br = 0;
         temp = 0;
         cnt = 0;
         FOR(z, 1, n){
            FOR(q, 1, n){
               if(temp_board[z][q]) temp += (1LL<<br);
               br++;
               if(br == 64){
                  cnt++;
                  mask_rows[i][cnt] = temp;
                  temp = 0;
                  br = 0;
               }
            }
         }
         if(br){
            cnt++;
            mask_rows[i][cnt] = temp;
         }

         //cols
         clr(temp_board, 0);
         FOR(j, 1, n){
            temp_board[j][i] = 1;
         }
         br = 0;
         temp = 0;
         cnt = 0;
         FOR(z, 1, n){
            FOR(q, 1, n){
               if(temp_board[z][q]) temp += (1LL<<br);
               br++;
               if(br == 64){
                  cnt++;
                  mask_cols[i][cnt] = temp;
                  temp = 0;
                  br = 0;
               }
            }
         }
         if(br){
            cnt++;
            mask_cols[i][cnt] = temp;
         }

      }
   }

   //preparing cordinates for every block and rotation
   void generate_blocks(){
      // xx
      // xx
      blocks[0][0][0] = mp(-1, 0);
      blocks[0][0][1] = mp(-1, -1);
      blocks[0][0][2] = mp(0, -1);

      blocks[0][1][0] = mp(0, 1);
      blocks[0][1][1] = mp(-1, 0);
      blocks[0][1][2] = mp(-1, 1);

      // .x
      // xx
      blocks[1][0][0] = mp(-1, 0);
      blocks[1][0][1] = mp(0, -1);
      blocks[1][0][2] = mp(0, 0);

      blocks[1][1][0] = mp(-1, 0);
      blocks[1][1][1] = mp(0, +1);
      blocks[1][1][2] = mp(0, 0);

      // ..x
      // xxx
      blocks[2][0][0] = mp(-1, 0);
      blocks[2][0][1] = mp(0, -1);
      blocks[2][0][2] = mp(0, -2);

      blocks[2][1][0] = mp(0, 1);
      blocks[2][1][1] = mp(-1, 0);
      blocks[2][1][2] = mp(-2, 0);

      // xxxx
      blocks[3][0][0] = mp(0, -1);
      blocks[3][0][1] = mp(0, -2);
      blocks[3][0][2] = mp(0, -3);

      blocks[3][1][0] = mp(-1, 0);
      blocks[3][1][1] = mp(-2, 0);
      blocks[3][1][2] = mp(-3, 0);

      // .x.
      // xxx
      blocks[4][0][0] = mp(-1, 0);
      blocks[4][0][1] = mp(-2, 0);
      blocks[4][0][2] = mp(-1, -1);

      blocks[4][1][0] = mp(0, 1);
      blocks[4][1][1] = mp(0, 2);
      blocks[4][1][2] = mp(-1, +1);

      //rotation
      FOR(j, 0, 4){
         FOR(i, 0, 2){
            blocks[j][2][i] = mp(-1*blocks[j][0][i].x, -1*blocks[j][0][i].y);
            blocks[j][3][i] = mp(-1*blocks[j][1][i].x, -1*blocks[j][1][i].y);
         }
      }
   }

   // creating mask for given block on the position r, c
   void make_new(int& block, int& rot, int& r, int& c){
      clr(temp_board, 0);
      FOR(i, 0, 2){
         temp_board[r+blocks[block][rot][i].x][c+blocks[block][rot][i].y] = 1;
      }
      temp_board[r][c] = 1;
      br = 0;
      temp = 0;
      cnt = 0;
      FOR(i, 1, n){
         FOR(j, 1, n){
            if(temp_board[i][j]) temp += (1LL<<br);
            br++;
            if(br == 64){
               cnt++;
               moves[block][rot][r][c][cnt] = temp;
               temp = 0;
               br = 0;
            }
         }
      }
      if(br){
         cnt++;
         moves[block][rot][r][c][cnt] = temp;
         temp = 0;
         br = 0;
      }
      moves[block][rot][r][c][0] = cnt;
      ntable = cnt;
   }

   //searches every possible way for placing every block
   void generate_binary_blocks(){
      bool exists;
      FOR(block, 0, 4){
         FOR(rot, 0, 3){
            FOR(i, 1, n){
               FOR(j, 1, n){
                  exists = 1;
                  FOR(z, 0, 2){
                     nr = i+blocks[block][rot][z].x;
                     nc = j+blocks[block][rot][z].y;
                     if(!in_bounds(nr, nc)){
                        exists = 0;
                        break;
                     }
                  }
                  if(!exists) continue;
                  make_new(block, rot, i, j);
               }
            }

         }
      }
   }

   //generates surfaze minimisation masks
   void generate_frames(){
      FOR(row, 1, n){
         FOR(col, 1, n){
            // only 1 in middle
            clr(temp_board, 0);
            temp_board[row][col] = 1;

            br = 0;
            temp = 0;
            cnt = 0;
            nframes++;
            FOR(i, 1, n){
               FOR(j, 1, n){
                  if(temp_board[i][j]) temp += (1LL<<br);
                  br++;
                  if(br == 64){
                     cnt++;
                     frames[nframes][0][cnt] = temp;
                     temp = 0;
                     br = 0;
                  }
               }
            }
            if(br){
               cnt++;
               frames[nframes][0][cnt] = temp;
               temp = 0;
               br = 0;
            }
            // around 1
            clr(temp_board, 0);
            FOR(z, 0, 3){
               nr = row + sr4[z];
               nc = col + sc4[z];
               if(!in_bounds(nr, nc)) continue;

               temp_board[nr][nc] = 1;
            }

            br = 0;
            temp = 0;
            cnt = 0;

            FOR(i, 1, n){
               FOR(j, 1, n){
                  if(temp_board[i][j]) temp += (1LL<<br);
                  br++;
                  if(br == 64){
                     cnt++;
                     frames[nframes][1][cnt] = temp;
                     temp = 0;
                     br = 0;
                  }
               }
            }
            if(br){
               cnt++;
               frames[nframes][1][cnt] = temp;
               temp = 0;
               br = 0;
            }
         }
      }
   }
}binaries;

struct IO{
   ///THIS CLASS IS ONLY FOR INPUT AND OUTPUT
   //inputs given paramteres
   ll a, b, d, e, C, F;
   void input(){
      END = clock() + 4500; // Start timer
      freopen("block.in", "r", stdin);
      cin >> n >> a >> b >> C >> d >> e >> F;
      max_depth = N[(n-1)/8];
      max_surf_const = 8+12*(n-2)+4*(n-2)*(n-2);

      for(int i = 1; i < mxM; i++){
         C = (C ^ a) + b;
         F = (F ^ d) + e;

         c[i] = C%5;
         f[i] = F%4;
      }
   }

   //outputs solution
   void output(){
      cout << "Kraj: " << m << endl;
      freopen("block.out", "w", stdout);
      pr("%d\n", m);

      FOR(i, 1, m){
         pr("%d %d\n", res[i].x, res[i].y);
      }
   }
}io;

struct Managing{
   ///THIS CLASS MANAGES PLACING, POSSIBLE MOVES, AND CLEARING ROW/COLS OF TABLE
   int rows_to_clear[mxN], cols_to_clear[mxN], cnt_rows, cnt_cols;
   int block, rot;
   bool exist;

   inline bool possible_move(ll (&board)[mxBits], int& row, int& col, int& id){     ///resen
      block = c[id];
      rot = f[id];
      if(moves[block][rot][row][col][0] == 0) return 0;

      FOR(i, 1, moves[block][rot][row][col][0]){
         if((board[i] & moves[block][rot][row][col][i]) != 0)
            return 0;
      }

      return 1;
   }

   //clears row or columns that are full
   void clear_rows_cols(ll (&board)[mxBits]){   ///resen
      cnt_rows = cnt_cols = 0;
      int b = 1;
      FOR(i, 1, n){
         exist = 1;
         FOR(j, 1, ntable){
            if((board[j] & mask_rows[i][j]) != mask_rows[i][j]){
               exist = 0;
               break;
            }
         }
         if(exist) rows_to_clear[++cnt_rows] = i;

         exist = 1;
         FOR(j, 1, ntable){
            if((board[j] & mask_cols[i][j]) != mask_cols[i][j]){
               exist = 0;
               break;
            }
         }
         if(exist) cols_to_clear[++cnt_cols] = i;
      }

      FOR(i, 1, cnt_cols){
         FOR(j, 1, ntable){
            board[j] = (board[j] & (~mask_cols[cols_to_clear[i]][j]));
         }
      }

      FOR(i, 1, cnt_rows){
         FOR(j, 1, ntable){
            board[j] = (board[j] & (~mask_rows[rows_to_clear[i]][j]));
         }
      }
   }

   //places current block, rot on position
   void place(ll (&board)[mxBits], const int& row, const int& col, const int& id){  ///resen
      block = c[id];
      rot = f[id];
      FOR(i, 1, moves[block][rot][row][col][0]){
         board[i] = (board[i] | moves[block][rot][row][col][i]);
      }
      clear_rows_cols(board);
   }
}mg;

struct Node{
   long long total_score;              //total sum of child (HEURISTIC) scores
   int heuristic_score;                //heuristic score of table
   int visits;                         //number of visits
   int id;                             //id of given block

   float uct_score;                    //score of this Node

   bool is_fully_expanded;

   //int row, col;                    //used for expansion, when it reaches end, is_fully_expanded = 1
   int row, col;                    //represents current move
   ll board[mxBits];                //table

   Node* parent;
   vector<Node*> children;
   vector<pii> possible_children;

   Node(ll (&given_board)[mxBits], Node* parent = nullptr, int row = 0, int col = 0, int id = 1) :
      parent(parent), is_fully_expanded(0), id(id), row(row), col(col), total_score(0){

      memcpy(this->board, given_board, sizeof(this->board));

      FOR(i, 1, n){
         FOR(j, 1, n){
            if(mg.possible_move(board, i, j, id)){
               possible_children.pb(mp(i, j));
            }
         }
      }

      heuristic_score = h.calc_heuristic(board);
   }

   ~Node(){
      for (Node* child : children) {
         delete child;
      }
   }

   void output_possible_children(){
      cout << "Possible children for node: " << id << endl;
      for(pii p : possible_children){
         cout << p.x << " " << p.y << endl;
      }
   }

   void update_utc(){
      if(parent == nullptr){
         uct_score = (1.0*total_score + koren2*sqrt(log(visits)*visits))/visits;
      }
      else{
         uct_score = (1.0*total_score + koren2*sqrt(log(parent->visits)*visits))/visits;
      }
   }
};
Node* root;
Node* next_node;

struct MCST{
   double best_value, parent_visits, win_rate, uct_value;
   Node* best_child;
   Node* new_node;
   int possible_children_size, next_row, next_col, score, id;

   //finds best move for given node
   Node* search(){
      Node* node = root;

      while(node->is_fully_expanded) {
         node = best_uct(node);
      }

      if(node->uct_score == -FLOATINF){
         return nullptr;
      }

      possible_children_size = node->possible_children.size();

      //creating new node
      if (possible_children_size > 0){
         id = rand()%possible_children_size;

         next_row = node->possible_children[id].x;
         next_col = node->possible_children[id].y;
         swap(node->possible_children[id], node->possible_children[possible_children_size-1]);
         node->possible_children.pop_back();

         memcpy(board2, node->board, sizeof(board2));
         mg.place(board2, next_row, next_col, node->id);

         new_node = new Node(board2, node, next_row, next_col, node->id+1);

         new_node->parent = node;
         node->children.push_back(new_node);

         if(possible_children_size == 1){
            node->is_fully_expanded = 1;
         }

         node = new_node;
      }
      else{
         node->uct_score = -FLOATINF;
         return nullptr;
      }

      //backpropagation from newly added node
      backpropagate(node);

      return node;  // Return the selected/expanded node
   }

   void backpropagate(Node* node) {
      score = node->heuristic_score;
      while (node != nullptr) {
         node->visits++;
         node->total_score += score;

         node->update_utc();

         node = node->parent;
      }
   }

   //finds best child to explore based on given UCT formula
   Node* best_uct(Node* node){
      best_child = nullptr;
      best_value = -FLOATINF-10;

      for(Node* child : node->children) {
         if(child->uct_score > best_value) {
            best_value = child->uct_score;
            best_child = child;
         }
      }
      return best_child;
   }
}mcst;

void cout_tree(Node* node){
   cout << "\n" << node->id << " " << node->heuristic_score << endl;
   cout_board(node->board);

   for(Node* next: node->children){
      cout_tree(next);
   }
}

void solve(){
   clr(board2, 0);
   root = new Node(board2);
   next_node = nullptr;

   int max_iteration = 100;

   if(n <= 16) max_iteration = 1500;

   while(1){
      for (int i = 0; i < max_iteration; i++){
         mcst.search();
      }

      next_node = mcst.best_uct(root);
      for(Node* node : root->children){
         if(node == next_node) continue;
         delete node;
      }
      root = next_node;

      if(root == nullptr) break;
      if(root->uct_score == -FLOATINF) break;

      res[++m] = mp(root->row, root->col);
   }


   delete root;
}

int main(){
   io.input();

   binaries.init();

   solve();

   io.output();

   return 0;
}
