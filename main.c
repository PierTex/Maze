#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "settings.h"

#define MAX_SIZE 9999

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

    init_path(&path);
    char **maze = input_file(M, N);
    refresh();

    find_entrance_exit(maze, N, M);
    while (!check_finish())
        move_right_hand(maze, N, M, &path);
    printf("\n");
    mark_path(&path, maze);
    print_maze(maze, N, M);
    finish_AI(maze, N, &path);
}

void AI_random()
{
    int M, N;
    int best_score;
    int n_iterations = 20000;
    path_t path, best_path;
    score_t ratio;

    printf("Inserire in ordine i dati della matrice (input separati da una newline):\n");
    printf("- Numero di colonne\n");
    printf("- Numero di righe\n");
    printf("- Mappa\n\n");
    scanf(" %d", &M);
    scanf(" %d", &N);

    init_path(&best_path);
    best_path.size = MAX_SIZE;
    char **maze = input_file(M, N);
    refresh();

    char **maze_copy = malloc(N * sizeof(char *));
    for (int i = 0; i < N; ++i)
        maze_copy[i] = malloc(M * sizeof(char *));
    maze_copy = copy_matrix(N, M, maze_copy, maze);

    for (int i = 0; i < n_iterations; ++i)
    {
        find_entrance_exit(maze, N, M);
        init_path(&path);
        move_random(maze, N, M, &path);
        if (path.size <= (size_t)(M * N))
        {
            ratio.current = score(path.size) / path.size;
            if (i == 0 || (score(path.size) >= best_score && ratio.current > ratio.best))
            {
                best_score = score(path.size);
                ratio.best = best_score / path.size;
                if (best_path.size != path.size)
                {
                    best_path.size = path.size;
                    char *new_ptr = (char *)realloc(best_path.moves, best_path.size * sizeof(char) + 1);
                    if (!new_ptr)
                    {
                        free(best_path.moves);
                        exit(EXIT_FAILURE);
                    }
                    best_path.moves = new_ptr;
                }
                memcpy(best_path.moves, path.moves, path.size * sizeof(char));
            }
        }
        maze = copy_matrix(N, M, maze, maze_copy);
        free_path(&path);
    }
    printf("\n");
    mark_path(&best_path, maze);
    print_maze(maze, N, M);
    free_maze(maze_copy, N);
    finish_AI(maze, N, &best_path);
}

void interactive()
{
    coordinates_t size;

    size.x = 10;
    size.y = 20;

    char **maze = create_maze(size.x, size.y);

    while (!check_finish())
    {
        refresh();
        print_maze(maze, size.x, size.y);
        char movement = insert_move();
        move(movement, maze, size.x, size.y);
    }
    refresh();
    print_maze(maze, size.x, size.y);
    finish(maze, size.x);
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
#    T     #      #
#    $$$$  #      #
###################

*/