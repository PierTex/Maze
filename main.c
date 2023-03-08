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

    refresh();

    init_path(&path);
    char **maze = inputFile(M, N);
    find_entrance_exit(maze, N, M);
    while (!checkFinish())
        move_right_hand(maze, N, M, &path);
    printf("\n");
    printMaze(maze, N, M);
    finish_AI(maze, N, &path);
}

void AI_random()
{
    int M, N;
    int best_score;
    size_t n_iterations = 10000;
    path_t path, best_path;
    score_t ratio;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("- Numero di colonne\n");
    printf("- Numero di righe\n");
    printf("- Mappa\n\n");
    scanf(" %d", &M);
    scanf(" %d", &N);

    init_path(&best_path);
    char **maze = inputFile(M, N);
    refresh();

    char **maze_copy = malloc(N * sizeof(char *));
    for (int i = 0; i < N; ++i)
        maze_copy[i] = malloc(M * sizeof(char *));
    maze_copy = copy_matrix(N, M, maze_copy, maze);

    for (size_t i = 0; i < n_iterations; ++i)
    {
        find_entrance_exit(maze, N, M);
        init_path(&path);
        while (!checkFinish())
            move_random(maze, N, M, &path);
        if (score(path.size) >= 0)
        {
            ratio.current = score(path.size) / path.size;
            if (i == 0 || (score(path.size) >= best_score && ratio.current > ratio.best))
            {
                best_score = score(path.size);
                ratio.best = best_score / path.size;
                best_path.size = path.size;
                char *new_ptr = (char *)realloc(best_path.moves, best_path.size * sizeof(char) + 1);
                if (!new_ptr)
                {
                    free(best_path.moves);
                    exit(EXIT_FAILURE);
                }
                best_path.moves = new_ptr;
                memcpy(best_path.moves, path.moves, path.size * sizeof(char));
            }
        }
        maze = copy_matrix(N, M, maze, maze_copy);
        free_path(&path);
    }
    printf("\n");
    mark_path(&best_path, maze);
    printMaze(maze, N, M);
    freeMaze(maze_copy, N);
    finish_AI(maze, N, &best_path);
}

void interactive()
{
    coordinates_t size;

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
    printf("[1]\tModalita' Interattiva\n");
    printf("[2]\tModalita' AI Sempre a Destra\n");
    printf("[3]\tModalita' AI Random\n");
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
    case 3:
        AI_random();
        break;
    default:
        printf("\nErrore, modalita' non presente\n");
        break;
    }

    return 0;
}

/*

19
10
###################
o    #          $ #
#    #          $ #
#    #   ! #    $ #
#    #     #    $ #
#    #     #      _
#    #     #      #
#          #      #
#    $$$$  #      #
###################

*/