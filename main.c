#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "settings.h"

void AI_right_hand()
{
    int M, N;

    path_t path;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("- Numero di colonne\n");
    printf("- Numero di righe\n");
    printf("- Mappa\n\n");

    scanf(" %d", &M);
    scanf(" %d", &N);

    char **maze = inputFile(M, N);
    init_path(&path);
    find_entrance_exit(maze, N, M);
    while (!checkFinish())
    {
        move_right_hand(maze, N, M, &path);
    }
    printf("\n");
    printMaze(maze, N, M);
    finish_AI(maze, N, &path);
}

void AI_random()
{
    int M, N;
    path_t path;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("- Numero di colonne\n");
    printf("- Numero di righe\n");
    printf("- Mappa\n\n");

    scanf(" %d", &M);
    scanf(" %d", &N);

    init_path(&path);
    char **maze = inputFile(M, N);
    find_entrance_exit(maze, N, M);
    while (!checkFinish())
        move_random(maze, N, M, &path);
    finish_AI(maze, N, &path);
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
    // refresh();

    short choice;

    printf("\n\t\tMENU' DI GIOCO\n\n");
    printf("[1]\tModalita' Interattiva\n");
    printf("[2]\tModalita' AI Sempre a Destra\n");
    printf("[3]\tModalita' AI Random\n");
    printf("\nScegli una modalita': ");
    scanf(" %hd", &choice);

    // refresh();

    switch (choice)
    {
    case 1:
        interactive();
        break;
    case 2:
        AI_right_hand();
        break;
    case 3:
        AI_random();
        break;
    default:
        printf("\nErrore, modalita' non presente\n");
        break;
    }

    return 0;
}