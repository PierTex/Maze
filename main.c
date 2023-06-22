// 881996 887438 896569

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
    game_t game;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("- Numero di colonne\n");
    printf("- Numero di righe\n");
    printf("- Mappa\n\n");
    scanf(" %d", &M);
    scanf(" %d", &N);

    game.maze_size.x = N;
    game.maze_size.y = M;

    init_path(&path, &game);
    char **maze = inputFile(M, N);
    refresh();

    find_entrance_exit(maze, N, M, &game);
    while (!checkFinish(&game))
        move_right_hand(maze, N, M, &path, &game);
    printf("\n");
    mark_path(&path, maze, &game);
    printMaze(maze, N, M);
    game.final_score = score(&game);
    finish_AI(maze, N, &path, &game);
}

void AI_random()
{
    int M, N;
    int best_score = -1;
    size_t n_iterations = 10000;
    path_t path, best_path;
    game_t game;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("- Numero di colonne\n");
    printf("- Numero di righe\n");
    printf("- Mappa\n\n");
    scanf(" %d", &M);
    scanf(" %d", &N);

    game.maze_size.x = N;
    game.maze_size.y = M;

    init_path(&best_path, &game);
    char **maze = inputFile(M, N);
    refresh();

    char **maze_copy = malloc(N * sizeof(char *));
    for (int i = 0; i < N; ++i)
        maze_copy[i] = malloc(M * sizeof(char));

    game.snake_head = NULL;
    game.head = NULL;
    copy_matrix(N, M, maze_copy, maze, &game);

    for (size_t i = 0; i < n_iterations; ++i)
    {
        find_entrance_exit(maze, N, M, &game);
        init_path(&path, &game);
        while (!checkFinish(&game))
            move_random(maze, N, M, &path, &game);
        if (score(&game) >= 0)
        {
            if (i == 0 || score(&game) >= best_score)
            {
                best_score = score(&game);
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
        copy_matrix(N, M, maze, maze_copy, &game);
        free_path(&path);
    }
    game.final_score = best_score;
    printf("\n");
    mark_path(&best_path, maze, &game);
    printMaze(maze, N, M);
    freeMaze(maze_copy, N);
    finish_AI(maze, N, &best_path, &game);
}

void interactive()
{
    coordinates_t size;
    game_t game;

    game.maze_size.x = 10;
    game.maze_size.y = 20;
    game.steps = 0;

    char **maze = createMaze(game.maze_size.x, game.maze_size.y, &game);

    while (!checkFinish(&game))
    {
        refresh();
        printMaze(maze, game.maze_size.x, game.maze_size.y);
        char movement = insertMove(&game);
        move(movement, maze, game.maze_size.x, game.maze_size.y, &game);
    }
    refresh();
    printMaze(maze, game.maze_size.x, game.maze_size.y);
    finish(maze, game.maze_size.x, &game);
}

int main()
{
    srand(time(NULL));
    refresh();

    short choice;

    printf("\n\tMENU' DI GIOCO\n\n");
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
#    $$$  T#      #
###################

*/