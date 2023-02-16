#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "settings.h"

coordinates size;

int main()
{
    int identifier = 7;
    size.x = 10;
    size.y = 20;

    char **maze = createMaze(size.x, size.y);

    int **numMaze = createNumMaze(maze, size.x, size.y);
    

    while (!checkFinish())
    {
        //refresh();

        printNumMaze(numMaze, size.x, size.y);

        printMaze(maze, size.x, size.y);

        char movement = insertMove();

        move(movement, maze, numMaze, identifier++);
    }
    refresh();
    printMaze(maze, size.x, size.y);
    finish();

    return 0;
}