#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "settings.h"

coordinates size;

void AI_right_hand()
{
    int M, N;
    scanf(" %d", &M);
    scanf(" %d", &N);
    char **maze = inputFile(M, N);
    find_entrance_exit(maze, N, M);
    while (!checkFinish())
        move_right_hand(maze, N, M);
    printf("\n");
    printMaze(maze, N, M);
    finish(maze, N);
}

void interactive()
{
    size.x = 10;
    size.y = 20;

    char **maze = createMaze(size.x, size.y);

    while (!checkFinish())
    {
        refresh();
        printMaze(maze, size.x, size.y);
        char movement = insertMove();
        move(movement, maze, size.x, size.y);
    }
    refresh();
    printMaze(maze, size.x, size.y);
    finish(maze, size.x);
}

int main()
{
    srand(time(NULL));

    // interactive();
    AI_right_hand();

    return 0;
}