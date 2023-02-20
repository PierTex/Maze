#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node
{
    int x;
    int y;
    struct Node *next;
} list_t;

typedef struct
{
    int x;
    int y;
} coordinates;

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

int **createNumMaze(char **maze, int x, int y);

char **inputFile(int M, int N);

void printMaze(char **maze, int x, int y);

void printNumMaze(int **numMaze, int x, int y);

bool checkDigitDirection(char direction);

char insertMove();

bool checkFinish();

void updateSnake(int **numMaze, char **maze, int playerX, int playerY);

bool move(char direction, char **maze, int **numMaze, int identifier);

void finish();

int checkPoints();

int checkPoints();

void printPoints(int points);

list_t *snakeAppend(list_t *snake);

list_t *snakeResize(list_t *snake);

void refresh();

#endif