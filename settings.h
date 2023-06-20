#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * This struct create a cell containing two int variable that would be use as the
 * coordinates for our snake
 */
typedef struct
{
    int x;
    int y;
} coordinates_t;

/**
 * This struct create a node containing a variable of type struct coordinates and
 * a pointer to a node. This node will be used as the body of our snake
 */
typedef struct Node
{
    coordinates_t body;
    struct Node *next;
} list_t;

/**
 * This struct create a cell containg a pointer to a char variable and two size_t
 * variable, one that indicate the capacity of the path of the snake and one that
 * indicate its length
 */
typedef struct
{
    char *moves;
    size_t capacity;
    size_t size;
} path_t;

/**
 * This struct create a cell containing two int variables that stands for the actual
 * score of the game and the best score of the game
 */
typedef struct
{
    int current;
    int best;
} score_t;

/**
 * This struct create a cell containing three boolean variables that stands for the
 * three direction that the IA mode will check before doing the move
 */
typedef struct
{
    bool cell_1;
    bool cell_2;
    bool cell_3;
} neighbors_t;

typedef struct
{
    list_t *head;
    list_t *snake_head;
    coordinates_t entrance;
    coordinates_t ending;
    coordinates_t backup;
    coordinates_t maze_size;
    char automove;
    unsigned steps;
    int coins;
    int penalties;
    int drills;
    int final_score;
} game_t;

/**
 * This function is used to refresh the screen terminal after each movement
 */
void refresh();

/**
 * This function is used to add a newline according to the operative system we are using
 */
void new_line();

/**
 * This fuction is used to calculate the final score of the game
 * @param steps Indicates the number of steps that the snake has done in the entire game
 */
int score(game_t *game);

/**
 * This function is used to free the memory used for the allocation of the snake
 * @param *l Indicate the starting node of the snake (snake head)
 */
void freeSnake(list_t *l);

/**
 * This function is used to free the memory used to allocate the entire maze
 * @param **maze Indicate the double pointer variable that is used as the maze
 * @param x Indicates the number of row of the maze
 */
void freeMaze(char **maze, int x);

/**
 * This function is used to allocate the head of the snake at the beginning of the game
 */
void create_snake_head(game_t *game);

/**
 * This function create a node that rapresents the body of the snake and this will be add to the snake using the snakeAppend function
 */
list_t *create_body();

/**
 * This function is used to append a node to the snake body in case of reciving a dollar
 * @param *new_body Indicates the node the function will add to the body
 * @param x Indicates the value of the x coordinate of the node
 * @param y Indicate the value of the y coordinate of the node
 */
void snakeAppend(list_t *new_body, int x, int y, game_t *game);

/**
 * This fuction is used to perform the movement of the snake, moving its head and all its body
 * @param x Indicates the x coordinate of the new position for the head
 * @param y Indicate the y coordinate of the new position for the head
 */
void snakeMovement(int x, int y, game_t *game);

/**
 * This function is used to reduce the size of the snake body in the case of receiving a penalty or in case of hitting the body with the head
 */
void snakeShrink(game_t *game);

/**
 * This function is used to check if the coordinates of the head are equal to the coordinate of some node of the body. In this case the function
 * will call the snakeShrink function
 */
void snakeEatingHimself(game_t *game);

/**
 * This function is used to clear the snake from the maze in order to re-add it using the snakePrint function
 * @param **maze Indicates the maze to clear
 */
void snakeClear(char **maze, game_t *game);

/**
 * This function is used to print the updated snake in the cleared maze
 * @param **maze Indicates the maze on which the snake will be printed
 */
void snakePrint(char **maze, game_t *game);

/**
 * This function is used to create a maze with random walls, dollars, drills, penalties, entry and exit
 * @param x Indicates the x size of the maze
 * @param y Indiates the y size of the maze
 */
char **createMaze(int x, int y, game_t *game);

// Funzione che copia il contenuto di una matrice (maze_from) in un'altra (maze_to), utilizzata nella AI Random
void copy_matrix(int x, int y, char **maze_to, char **maze_from, game_t *game);

/**
 * This function is used to print the maze
 * @param **maze Indicates the maze the function will print
 * @param x Indicate the x size of the maze
 * @param y Indicates the y size of the maze
 */
void printMaze(char **maze, int x, int y);

/**
 * This function is used to take a maze from the command line input
 * @param M Indicates the x size of the maze
 * @param N Indicates the y size of the maze
 */
char **inputFile(int M, int N);

/**
 * This function is used to check if the inserted move is one of the four cardinal characters
 * @param direction Indicates the move to check
 */
bool checkDigitDirection(char direction);

/**
 * This function is used to print the value of drills, dollars and penalties taken and to take a move from input
 */
char insertMove(game_t *game);

/**
 * This function is used to check if the coordinates of the snake head are equal to the coordinates of the exit
 */
bool checkFinish(game_t *game);

/**
 * This function is used to print the string YOU WON when the head reaches the exit of the maze
 * @param **maze Indicates the maze to check
 * @param x Indicates the x size of the maze
 */
void finish(char **maze, int x, game_t *game);

/**
 * This function is used to check if the head is under a collecatable object
 * @param **maze Indicates the maze to check
 */
void check_collectable(char **maze, game_t *game);

/**
 * This function in used to check if the move in an illegal movemenent or not
 * @param direction Indicates the inserted character
 * @param **maze Indicates the maze on which performing the check
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 */
void move(char direction, char **maze, int x, int y, game_t *game);

/**
 * Specific functions for the AI mode
 */

/**
 * This function is used to free the memory of the path of the snake and to put path->capacity and path->size to zero
 * @param *path Indicates the path to free
 */
void free_path(path_t *path);

/**
 * This function is used to initzialise the *path variable and to set the values of coins, penalties and drills to zero
 * @param *path Indicates the path to initzialise
 */
void init_path(path_t *path, game_t *game);

/**
 * This function is used to add a move to the path and to expand the capacity if necessary
 * @param *path Indicates the path to expand
 */
void add_move(path_t *path, game_t *game);

/**
 * This function is used to print the path int the form NESONESO...
 * @param Indicates the path to print
 */
void print_path(path_t *path);

/**
 * This function is used to print the score and the path and to free all the memory once the game ended
 * @param **maze Indicates the maze to free
 * @param x Indicates the x size of the maze
 * @param *path Indicates the path to print and to free
 */
void finish_AI(char **maze, int x, path_t *path, game_t *game);

/**
 * This fuction is used to find the coordinates of the entrancce and the exit of the maze that has been passed from input
 * @param **maze Indicates the maze to check
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 */
void find_entrance_exit(char **maze, int x, int y, game_t *game);

/**
 * This function is used to check the first possible move following the concept of the Always Right mode
 * @param **maze Indicates the maze on which perform the mode
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 * @param *path Indicates the path to expand with the move
 */
void move_right_hand(char **maze, int x, int y, path_t *path, game_t *game);

/**
 * This function is used to check the cell near the head to perform the AI Random mode
 * @param **maze Indicates the maze on which perform the mode
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 * @param cells Indicates the three cell to check
 */
void check_near_cells_N(char **maze, int y, int *cell, neighbors_t cells, game_t *game);

/**
 * This function is used to check the cell near the head to perform the AI Random mode
 * @param **maze Indicates the maze on which perform the mode
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 * @param cells Indicates the three cell to check
 */
void check_near_cells_S(char **maze, int x, int y, int *cell, neighbors_t cells, game_t *game);

/**
 * This function is used to check the cell near the head to perform the AI Random mode
 * @param **maze Indicates the maze on which perform the mode
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 * @param cells Indicates the three cell to check
 */
void check_near_cells_E(char **maze, int x, int y, int *cell, neighbors_t cells, game_t *game);

/**
 * This function is used to check the cell near the head to perform the AI Random mode
 * @param **maze Indicates the maze on which perform the mode
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 * @param cells Indicates the three cell to check
 */
void check_near_cells_O(char **maze, int y, int *cell, neighbors_t cells, game_t *game);

/**
 * This function is used to move the snake in the AI Random mode
 * @param **maze Indicates the maze on which perform the move
 * @param x Indicates the x size of the maze
 * @param y Indicates the y size of the maze
 * @param *path Indicates the path to expand
 */
void move_random(char **maze, int x, int y, path_t *path, game_t *game);

/**
 * This function is used to mark the path done by the snake during the AI Random or the AI Always Right
 * @param *path Indicates the path to mark with dots
 * @param **maze Indicates the maze on which mark the path
 */
void mark_path(path_t *path, char **maze, game_t *game);

#endif