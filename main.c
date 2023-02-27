#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "settings.h"

void AI_right_hand()
{
    int M, N;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("\tNumero di colonne\n");
    printf("\tNumero di righe\n");
    printf("\tMappa\n\n");

    scanf(" %d", &M);
    scanf(" %d", &N);
    char **maze = inputFile(M, N);
    find_entrance_exit(maze, N, M);
    while (!checkFinish())
        move_right_hand(maze, N, M);
    printf("\n");
    printMaze(maze, N, M);
    finish_AI(maze, N);
}

void AI_random()
{
    int M, N;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("\tNumero di colonne\n");
    printf("\tNumero di righe\n");
    printf("\tMappa\n\n");

    scanf(" %d", &M);
    scanf(" %d", &N);
    char **maze = inputFile(M, N);
    find_entrance_exit(maze, N, M);
    while(!checkFinish())
    {
        //
    }
    finish_AI(maze, N);
}

void interactive()
{
    coordinates size;

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
    refresh();

    short choice;

    printf("\n\t\tMENU' DI GIOCO\n\n");
    printf("[1]\tModalita' interattiva\n");
    printf("[2]\tModalita' AI (sempre a destra)\n");
    printf("\nScegli una modalita': ");
    scanf(" %hd", &choice);

    refresh();

    switch (choice)
    {
    case 1:
        interactive();
        break;
    case 2:
        AI_right_hand();
        break;
    default:
        printf("\nErrore, modalita' non presente\n");
        break;
    }

    return 0;
}