#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int x;
    int y;
} coordinates;

typedef struct Node
{
    coordinates body;
    struct Node *next;
} list_t;

typedef struct
{
    char *moves;
    size_t capacity;
    size_t size;
} path_t;

// COLORS

void resetColor();
void black();
void red();
void green();
void yellow();
void blue();
void purple();
void cyan();
void white();

char **createMaze(int x, int y);
char **inputFile(int M, int N);
void printMaze(char **maze, int x, int y);
bool checkDigitDirection(char direction);
char insertMove();
bool checkFinish();
void move(char direction, char **maze, int x, int y);
void finish(char **maze, int x);
int checkPoints();
void printPoints(int points);
void snakeAppend(list_t *new_body, int x, int y);
void snakeShrink();
void refresh();

void AI_right_hand();
void init_path(path_t *path);
void find_entrance_exit(char **maze, int x, int y);
void move_right_hand(char **maze, int x, int y, path_t *path);
void move_random(char **maze, int x, int y, path_t *path);
void finish_AI(char **maze, int x, path_t *path);
void add_move(path_t *path);
void print_path(path_t *path);

#endif