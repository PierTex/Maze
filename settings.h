#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{

    int x;
    int y;
    struct Node *next;
} list_t;

typedef struct Exit{
    int x;
    int y;
} coordinate;

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

bool move(char direction, char **maze); 

void finish();



int checkPoints();

int checkPoints();

void printPoints(int points);

list_t *snakeAppend(list_t *snake);

list_t *snakeResize(list_t *snake);

void refresh();





#endif

