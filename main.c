#include "Settings.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

int main(){


    return 0;
}

char **createMaze();

char **inputFile();

void printMaze(char **maze);

char insertMove();

bool checkMove(char move); // anche raccoglibili 

int checkPoints();

int points();

void printPoints(int points);

list_t *snakeAppend(list_t *snake);

list_t *snakeResize(list_t *snake);

void refresh();