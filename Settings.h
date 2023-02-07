#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct Node{

    int x;
    int y;
    struct Node *next;
} list_t;

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





#endif