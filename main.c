#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "settings.h"

coordinates size;

int main()
{
    size.x = 10;
    size.y = 20;

    char **maze = createMaze(size.x, size.y);

    while (!checkFinish())
    {
        refresh();
        printMaze(maze, size.x, size.y);
        char movement = insertMove();
        move(movement, maze);
    }

    return 0;
}