#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "settings.h"

int sizeX = 10;
int sizeY = 20;

int main() {

    char **maze = createMaze(sizeX, sizeY);

    while(!checkFinish()) {
        printMaze(maze, sizeX, sizeY);
        char movement = insertMove();
        move(movement, maze);
    }

    




    return 0;
}