# Monte Carlo Tree Search AI for 1010!

## 📌Project Overview
This is the C++ implementation of **MCTS** (Monte Carlo Search Tree) algorithm for finding optimal moves in playing 1010! game.

## What is 1010! game
1010! is a block puzzle game similar to Tetris but without gravity. The goal is to place Tetromino-like shapes on a 10x10 grid to form and clear full rows or columns. The game ends when no more moves are possible. You get 3 block-shapes per turn.

![1010 Example](images\1010.PNG)
> https://poki.com/en/g/1010-deluxe?msockid=32a5d5abcfa16b332995c0e1cebc6a1d

## 🧠MCTS Algorithm
Monte Carlo Search Tree is a heuristic search algorithm for exploring moves and possibility outcomes. It is similar to **minimax algorithm**.

Main features of this algorithm is it doesnt need to procees all possible moves from one stage (**node**), but it chooses when to explore new moves, when to exploite already procceded moves and check their value.

It consists of 4 steps:
 * **Selection**: In this process, the MCTS algorithm traverses the current tree from the root node using a specific strategy. As I mentioned, it balances exploration and exploiation with this formula called **Upper Confidence Bound (UCB)**:

    $$ S_i = x_i + C\sqrt{ \frac{ln(t)}{n_i}} $$
    * $S_i$ is value of a node $i$, UCT score
    * $x_i$ is empirical mean of a node $i$, (*total sum of scores / number of visits*)
    *  $C$ is constant, usually $\sqrt2$
    * $t$ is total number of iterations
    * $n_i$ number of visits of node $i$

* **Expansion** Appending new child ("performing new move") to the node chosen with **Selection** process

* **Simulation** In this process, a simulation is performed by choosing random moves until a result or predefined state is achieved.

>**In this particular problem, since it is not 2 player game, heuristic score of current state in that node is calculated**

* **Backpropagation** After calculating value of new child, with backpropagation we update remaining tree, from child, to root (from bottom top top).


## 🧩Problem statement
Every block consists of а few squares, each covering 1 cell in the matrix. The lighter colored square is special and is used for defining the position of a move. Every block that you receive is rotated by 0, 90, 180 or 270 degrees clockwise. 

If:
1. the block cannot be placed on any free cells;
2. the player gives up or 
3. the time runs out

 the **game ends** and the player’s score is the count of moves until now. 

The committee chooses n, denoting dimension of table and 6 integers: a, b, c, d, e, f. Before each turn the following operations 
are performed: 
c = (c XOR a) + b 
f = (f XOR d) + e 

After that the block’s index is c mod 5, while its rotation is f mod 4. The rotation is 90 * (f mod 4) degrees. 

![block image](images\blocks.PNG)
### Constraints:
$n = {8, 16, 20, 25}$

$0 <= a, b, c, d, e, f <= 10^9$

## 🔧Code Structure
### IO class
Used only for input output methods. Inputs from *block.in* and writes to *block.out*

### Binary class
Because for representing board, it is only matter if cell is empty or filled, instead of using boolean array or matrix, I used bitmasks. It is method in which every bit of integer (long long integer) represents cell in board. It contributes to efficient executing, **because with one bitwise operation we replace 64 operations**.

#### generate_blocks
Manually generation of every block and every rotation, then marking what cells it covers.

#### generate_masks
 Generates masks for covering each row and each column

 #### generate_frames
 Generates "frame", which represents empty cell, and then cells that surround it.

### Heuristic class
It is used for calculating heuristic value of board. It considers 4 things:
 * surface minimization
 * empty cells
 * alignment maximization
 * isolated cells
With parameters $A, D, E , F$ we can adjust the score and infuense of particular segment on the overall score.

### Managing class
It handles operation on the boards:

**intersection**

    if(board & piece)

**clear rows or cols**

    board = (board & row_mask) 

**placing**

    board = (board | piece)

Depends on what we need, we just need to create needed mask and combine those tehniques.

### MCTS class
It implements **selection** (search), **backpropagate** and finding **best uct node**.

### Node class
This is node for the tree, it represents **state**. It has modified constructor, and some attributes including total_score, heuristic_score, number_of_visits, board... also pointers for parent node, and vector representing all possible childrens.

## 📚 References
* https://www.geeksforgeeks.org/ml-monte-carlo-tree-search-mcts/
* https://blog.coelho.net/1010-analysis/#game-presentation

