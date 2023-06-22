// 881996 887438 896569

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "settings.h"

#define INIT_CAPACITY 10
#define GROWTH_FACTOR 2

#define GREEN_LIME "\x1B[38;2;50;205;50m"
#define RED_CRIMSON "\x1B[38;2;220;20;60m"
#define GREY_GAINSBORO "\x1B[38;2;220;220;220m"
#define YELLOW "\x1B[38;2;255;255;0m"
#define BROWN_BURLYWOOD "\x1B[38;2;222;184;135m"
#define CYAN "\x1B[38;2;0;255;255m"
#define COLOR_RESET "\x1B[0m"

void refresh()
{
#ifdef _WIN32
    system("cls");
#endif

#ifdef __APPLE__
    system("clear");
#endif

#ifdef __linux__
    system("clear");
#endif
}

void new_line()
{
#ifdef __APPLE__
    printf("\n");
#endif

#ifdef __linux__
    printf("\n");
#endif
}

int score(game_t *game)
{
    int max = 1000;
    return max + game->coins * 10 - game->steps;
}

void freeSnake(list_t *l)
{
    list_t *tmp;
    while (l)
    {
        tmp = l;
        l = l->next;
        free(tmp);
    }
}

void freeMaze(char **maze, int x)
{
    for (int i = 0; i < x; ++i)
        free(maze[i]);
    free(maze);
}

char **createMaze(int x, int y, game_t *game)
{
    game->coins = 10;
    game->penalties = 5;
    game->drills = 2;

    char **maze = malloc(x * sizeof(char *));

    for (int i = 0; i < x; i++)
        maze[i] = malloc(y * sizeof(char *));

    int nWalls, pHole;
    int lenCorridor = ((rand() % 3) + 2); // larghezza corridoio (2 o 3 spazi)

    // creazione entrata e uscita
    game->entrance.x = rand() % (x - 2) + 1;
    game->entrance.y = 0;
    game->ending.x = rand() % (x - 2) + 1;
    game->ending.y = y - 1;

    // stampa bordo
    bool frame;         // controllo bordo labirinto
    bool entrance_exit; // controllo entrata o uscita
    for (int row = 0; row < x; ++row)
    {
        for (int col = 0; col < y; ++col)
        {
            frame = (row == 0 || row == x - 1 || col == 0 || col == y - 1);
            entrance_exit = ((col == game->entrance.y && row == game->entrance.x) || (col == game->ending.y && row == game->ending.x));
            if (frame && !entrance_exit)
                maze[row][col] = '#';
            else
            {
                maze[row][col] = ' ';
                if (col == game->entrance.y && row == game->entrance.x)
                {
                    create_snake_head(game);
                    game->head->body.x = game->entrance.x;
                    game->head->body.y = game->entrance.y;

                    maze[game->head->body.x][game->head->body.y] = '@';
                }
                if (col == game->ending.y && row == game->ending.x)
                    maze[row][col] = '_';
            }
        }
    }

    // stampa delle mura piene
    nWalls = y / 4;
    for (int col = 3, ctr = 0; ctr < nWalls; col += lenCorridor, ctr++, col++)
    {
        if (col < y - 7)
        {
            lenCorridor = rand() % (3) + 2;
            for (int row = 1; row < x - 1; ++row)
                maze[row][col] = '#';
        }
        if (col >= y - 7 && col <= y - 4)
        {
            lenCorridor = 2;
            for (int row = 1; row < x - 1; ++row)
                maze[row][col] = '#';
        }
    }

    // creazione buchi per colonna
    int k;
    bool check;    // posso creare un buco?
    bool atLeast1; // e' presente almeno un buco?
    for (int col = 3; col < y - 2; col++)
    {
        if (maze[1][col] == '#')
        { // siamo in una colonna?
            atLeast1 = false;
            do
            {
                for (int row = 1; row < x - 2; row++)
                {
                    pHole = rand() % (7); // probabilita' creazione buco
                    check = (maze[row][col] == '#' && maze[row - 1][col] == '#');
                    if (check)
                        if (pHole == 0 && row < x - 3)
                        {
                            maze[row][col] = ' ';
                            maze[++row][col] = ' ';
                            row++;
                            atLeast1 = true;
                        }
                }
            } while (!atLeast1);
        }
    }

    // generazione monete, penalita' e trapani
    coordinates_t coin, penalty, drill;
    bool isAvailableCoin, isAvailablePenalty, isAvailableDrill; // controllo cella
    while (game->coins || game->penalties || game->drills)
    {
        if (game->coins > 0)
        {
            do
            {
                coin.x = rand() % (x - 2) + 1;
                coin.y = rand() % (y - 2) + 1;
                isAvailableCoin = (maze[coin.x][coin.y] == ' ' && !(coin.x == game->entrance.x && coin.y == game->entrance.y) && !(coin.x == game->ending.x && coin.y == game->ending.y));
            } while (!isAvailableCoin);
            maze[coin.x][coin.y] = '$';
            game->coins--;
        }
        if (game->penalties > 0)
        {
            do
            {
                penalty.x = rand() % (x - 2) + 1;
                penalty.y = rand() % (y - 2) + 1;
                isAvailablePenalty = (maze[penalty.x][penalty.y] == ' ' && !(penalty.x == game->entrance.x && penalty.y == game->entrance.y) && !(penalty.x == game->ending.x && penalty.y == game->ending.y));
            } while (!isAvailablePenalty);
            maze[penalty.x][penalty.y] = '!';
            game->penalties--;
        }
        if (game->drills > 0)
        {
            do
            {
                drill.x = rand() % (x - 2) + 1;
                drill.y = rand() % (y - 2) + 1;
                isAvailableDrill = (maze[drill.x][drill.y] == ' ' && !(drill.x == game->entrance.x && drill.y == game->entrance.y) && !(drill.x == game->ending.x && drill.y == game->ending.y));
            } while (!isAvailableDrill);
            maze[drill.x][drill.y] = 'T';
            game->drills--;
        }
    }

    return maze;
}

void create_snake_head(game_t *game)
{
    game->snake_head = NULL;
    game->snake_head = (list_t *)malloc(sizeof(list_t));
    if (!game->snake_head)
    {
        printf("\nErrore, allocamento memoria fallito\n");
        exit(EXIT_FAILURE);
    }
    game->head = game->snake_head;
    game->head->next = NULL;
}

list_t *create_body()
{
    list_t *new_body = (list_t *)malloc(sizeof(list_t));
    if (!new_body)
    {
        printf("\nErrore, allocamento memoria fallito\n");
        exit(EXIT_FAILURE);
    }
    return new_body;
}

void snakeAppend(list_t *new_body, int x, int y, game_t *game)
{
    game->head->next = new_body;
    new_body->body.x = x;
    new_body->body.y = y;
    new_body->next = NULL;
}

void snakeMovement(int x, int y, game_t *game)
{
    coordinates_t last_pos, current_pos;
    last_pos.x = game->head->body.x;
    last_pos.y = game->head->body.y;
    game->head->body.x += x;
    game->head->body.y += y;
    while (game->head->next)
    {
        game->head = game->head->next;
        current_pos.x = game->head->body.x;
        current_pos.y = game->head->body.y;
        game->head->body.x = last_pos.x;
        game->head->body.y = last_pos.y;
        last_pos.x = current_pos.x;
        last_pos.y = current_pos.y;
    }
    game->backup.x = last_pos.x;
    game->backup.y = last_pos.y;
    game->steps++;
}

void snakeShrink(game_t *game)
{
    int i = 0;
    list_t *ptrBackup;
    do
    {
        ptrBackup = game->head;
        game->head = game->head->next;
        i++;
    } while (i <= game->coins);
    freeSnake(game->head);
    game->head = ptrBackup;
    game->head->next = NULL;
}

void snakeEatingHimself(game_t *game)
{
    int i = 0;
    bool found = false;
    game->head = game->snake_head;
    game->head = game->head->next;
    while (game->head && !found)
    {
        if (game->snake_head->body.x == game->head->body.x &&
            game->snake_head->body.y == game->head->body.y)
        {
            found = true;
            game->coins = i;
        }
        else
            i++;
        game->head = game->head->next;
    }
    game->head = game->snake_head;
    if (found)
        snakeShrink(game);
}

void snakeClear(char **maze, game_t *game)
{
    game->head = game->snake_head;
    while (game->head)
    {
        maze[game->head->body.x][game->head->body.y] = ' ';
        game->head = game->head->next;
    }
    game->head = game->snake_head;
}

void snakePrint(char **maze, game_t *game)
{
    game->head = game->snake_head;
    maze[game->head->body.x][game->head->body.y] = '@';
    game->head = game->head->next;
    while (game->head)
    {
        maze[game->head->body.x][game->head->body.y] = 'o';
        game->head = game->head->next;
    }
    game->head = game->snake_head;
}

void copy_matrix(int x, int y, char **maze_to, char **maze_from, game_t *game)
{
    for (int i = 0; i < x; ++i)
        for (int j = 0; j < y; ++j)
            maze_to[i][j] = maze_from[i][j];

    freeSnake(game->snake_head);
    game->snake_head = NULL;
}

void printMaze(char **maze, int x, int y)
{
    char elem;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            elem = maze[i][j];
            if (elem == '@' || elem == 'o')
                printf(CYAN "%c" COLOR_RESET, elem);
            else if (elem == '#' || elem == '_')
                printf(GREEN_LIME "%c" COLOR_RESET, elem);
            else if (elem == '$')
                printf(YELLOW "%c" COLOR_RESET, elem);
            else if (elem == '!')
                printf(RED_CRIMSON "%c" COLOR_RESET, elem);
            else if (elem == 'T')
                printf(BROWN_BURLYWOOD "%c" COLOR_RESET, elem);
            else
                printf("%c", elem);
        }
        printf("\n");
    }
}

char **inputFile(int M, int N)
{
    char *line;
    char **maze;

    line = malloc((M + 2) * sizeof(char *));
    maze = malloc(N * sizeof(char *));

    for (int i = 0; i < N; ++i)
        maze[i] = malloc(M * sizeof(char *));

    char bin;
    scanf("%c", &bin);

    for (int i = 0; i < N; ++i)
    {
        fgets(line, M + 2, stdin);
        for (int j = 0; j < M; ++j)
            maze[i][j] = line[j];
    }

    free(line);
    return maze;
}

bool checkDigitDirection(char direction)
{
    if (tolower(direction) == 'n' ||
        tolower(direction) == 's' ||
        tolower(direction) == 'e' ||
        tolower(direction) == 'o')
        return true;
    return false;
}

char insertMove(game_t *game)
{
    char direction;

    printf("\nMonete:\t\t%d\nPenalita':\t%d\nTrapani:\t%d\n\n", game->coins, game->penalties, game->drills);
    printf("Inserisci mossa: ");
    scanf("%c", &direction);
    fflush(stdin);

    while (!checkDigitDirection(direction))
    {
        scanf("%c", &direction);
        fflush(stdin);
    }

    return direction;
}

void finish(char **maze, int x, game_t *game)
{
    freeSnake(game->snake_head);
    freeMaze(maze, x);
    printf("\nHai vinto!!!\nPunteggio: %d\n", score(game));
    new_line();
}

bool checkFinish(game_t *game)
{
    if (game->head->body.x == game->ending.x && game->head->body.y == game->ending.y)
        return true;
    return false;
}

void check_collectable(char **maze, game_t *game)
{
    if (maze[game->snake_head->body.x][game->snake_head->body.y] == '$')
    {
        snakeAppend(create_body(), game->backup.x, game->backup.y, game);
        game->coins++;
    }
    else if (maze[game->snake_head->body.x][game->snake_head->body.y] == '!')
    {
        if (game->coins)
        {
            game->head = game->snake_head;
            game->coins /= 2;
            snakeShrink(game);
        }
        game->penalties++;
    }
    else if (maze[game->snake_head->body.x][game->snake_head->body.y] == 'T')
        game->drills += 3;
    else
    {
        if (game->coins > 1)
            snakeEatingHimself(game);
    }
}

void move(char direction, char **maze, int x, int y, game_t *game)
{
    bool supreme_wall;
    game->head = game->snake_head;
    snakeClear(maze, game);
    switch (tolower(direction))
    {
    case 'n':
        supreme_wall = game->snake_head->body.x - 1 < 0;
        if (!supreme_wall && (maze[game->snake_head->body.x - 1][game->snake_head->body.y] != '#' || game->drills))
            snakeMovement(-1, 0, game);
        break;
    case 's':
        supreme_wall = game->snake_head->body.x + 1 > x - 1;
        if (!supreme_wall && (maze[game->snake_head->body.x + 1][game->snake_head->body.y] != '#' || game->drills))
            snakeMovement(+1, 0, game);
        break;
    case 'e':
        supreme_wall = game->snake_head->body.y + 1 > y - 1;
        if (!supreme_wall && (maze[game->snake_head->body.x][game->snake_head->body.y + 1] != '#' || game->drills))
            snakeMovement(0, +1, game);
        break;
    case 'o':
        supreme_wall = game->snake_head->body.y - 1 < 0;
        if (!supreme_wall && (maze[game->snake_head->body.x][game->snake_head->body.y - 1] != '#' || game->drills))
            snakeMovement(0, -1, game);
        break;
    }
    if (maze[game->snake_head->body.x][game->snake_head->body.y] == '#' && game->drills)
        game->drills--;
    else
        check_collectable(maze, game);
    snakePrint(maze, game);
}

// Funzioni AI

void free_path(path_t *path)
{
    free(path->moves);
    path->capacity = 0;
    path->size = 0;
}

void init_path(path_t *path, game_t *game)
{
    path->moves = (char *)malloc(INIT_CAPACITY * sizeof(char) + 1);
    path->capacity = INIT_CAPACITY;
    path->size = 0;
}

void add_move(path_t *path, game_t *game)
{
    if (path->size == path->capacity)
    {
        path->capacity *= GROWTH_FACTOR;
        char *new_ptr = (char *)realloc(path->moves, path->capacity * sizeof(char) + 1);
        if (!new_ptr)
        {
            free(path->moves);
            exit(EXIT_FAILURE);
        }
        path->moves = new_ptr;
    }
    path->moves[path->size++] = game->automove;
}

void print_path(path_t *path)
{
    for (size_t i = 0; i < path->size; i++)
        printf("%c", path->moves[i]);
    printf("\n");
}

void finish_AI(char **maze, int x, path_t *path, game_t *game)
{
    printf("\nPunteggio: %d\nPercorso effettuato: ", game->final_score);
    path->moves[path->size] = '\0';
    print_path(path);
    new_line();
    free_path(path);
    freeSnake(game->snake_head);
    freeMaze(maze, x);
}

void find_entrance_exit(char **maze, int x, int y, game_t *game)
{
    game->coins = 0;
    game->penalties = 0;
    game->drills = 0;
    game->steps = 0;

    create_snake_head(game);

    for (int row = 0; row < x; ++row)
        for (int col = 0; col < y; ++col)
        {
            if (maze[row][col] == 'o')
            {
                game->entrance.x = row;
                game->entrance.y = col;
                if (game->entrance.x == 0)
                    game->automove = 'S';
                else if (game->entrance.x == x - 1)
                    game->automove = 'N';
                else if (game->entrance.y == 0)
                    game->automove = 'E';
                else if (game->entrance.y == y - 1)
                    game->automove = 'O';
                game->snake_head->body.x = row;
                game->snake_head->body.y = col;
            }
            if (maze[row][col] == '_')
            {
                game->ending.x = row;
                game->ending.y = col;
            }
        }
}

void move_right_hand(char **maze, int x, int y, path_t *path, game_t *game)
{
    game->head = game->snake_head;
    switch (toupper(game->automove))
    {
    case 'N':
        if (!(game->snake_head->body.y + 1 > y - 1) &&
            (maze[game->snake_head->body.x][game->snake_head->body.y + 1] != '#' || game->drills))
        {
            game->automove = 'E';
            snakeMovement(0, +1, game);
        }
        else if (!(game->snake_head->body.x - 1 < 0) &&
                 (maze[game->snake_head->body.x - 1][game->snake_head->body.y] != '#' || game->drills))
        {
            snakeMovement(-1, 0, game);
        }
        else if (!(game->snake_head->body.y - 1 < 0) &&
                 (maze[game->snake_head->body.x][game->snake_head->body.y - 1] != '#' || game->drills))
        {
            game->automove = 'O';
            snakeMovement(0, -1, game);
        }
        else
        {
            game->automove = 'S';
            snakeMovement(+1, 0, game);
        }
        break;
    case 'S':
        if (!(game->snake_head->body.y - 1 < 0) &&
            (maze[game->snake_head->body.x][game->snake_head->body.y - 1] != '#' || game->drills))
        {
            game->automove = 'O';
            snakeMovement(0, -1, game);
        }
        else if (!(game->snake_head->body.x + 1 > x - 1) &&
                 (maze[game->snake_head->body.x + 1][game->snake_head->body.y] != '#' || game->drills))
        {
            snakeMovement(+1, 0, game);
        }
        else if (!(game->snake_head->body.y + 1 > y - 1) &&
                 (maze[game->snake_head->body.x][game->snake_head->body.y + 1] != '#' || game->drills))
        {
            game->automove = 'E';
            snakeMovement(0, +1, game);
        }
        else
        {
            game->automove = 'N';
            snakeMovement(-1, 0, game);
        }
        break;
    case 'E':
        if (!(game->snake_head->body.x + 1 > x - 1) &&
            (maze[game->snake_head->body.x + 1][game->snake_head->body.y] != '#' || game->drills))
        {
            game->automove = 'S';
            snakeMovement(+1, 0, game);
        }
        else if (!(game->snake_head->body.y + 1 > y - 1) &&
                 (maze[game->snake_head->body.x][game->snake_head->body.y + 1] != '#' || game->drills))
        {
            snakeMovement(0, +1, game);
        }
        else if (!(game->snake_head->body.x - 1 < 0) &&
                 (maze[game->snake_head->body.x - 1][game->snake_head->body.y] != '#' || game->drills))
        {
            game->automove = 'N';
            snakeMovement(-1, 0, game);
        }
        else
        {
            game->automove = 'O';
            snakeMovement(0, -1, game);
        }
        break;
    case 'O':
        if (!(game->snake_head->body.x - 1 < 0) &&
            (maze[game->snake_head->body.x - 1][game->snake_head->body.y] != '#' || game->drills))
        {
            game->automove = 'N';
            snakeMovement(-1, 0, game);
        }
        else if (!(game->snake_head->body.y - 1 < 0) &&
                 (maze[game->snake_head->body.x][game->snake_head->body.y - 1] != '#' || game->drills))
        {
            snakeMovement(0, -1, game);
        }
        else if (!(game->snake_head->body.x + 1 > x - 1) &&
                 (maze[game->snake_head->body.x + 1][game->snake_head->body.y] != '#' || game->drills))
        {
            game->automove = 'S';
            snakeMovement(+1, 0, game);
        }
        else
        {
            game->automove = 'E';
            snakeMovement(0, +1, game);
        }
        break;
    }
    add_move(path, game);
    if (maze[game->snake_head->body.x][game->snake_head->body.y] == '#' && game->drills)
    {
        if (game->automove == 'N')
            game->automove = 'O';
        else if (game->automove == 'S')
            game->automove = 'E';
        else if (game->automove == 'E')
            game->automove = 'N';
        else if (game->automove == 'O')
            game->automove = 'S';
        game->drills--;
    }
    else
        check_collectable(maze, game);
}

void check_near_cells_N(char **maze, int y, int *odds, neighbors_t cells, game_t *game)
{
    coordinates_t tmp;
    tmp.x = game->snake_head->body.x;
    tmp.y = game->snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            game->automove = 'S';
            snakeMovement(0, +1, game);
        }
        else
            switch (*odds)
            {
            case 1:
                if (game->snake_head->body.y - 1 < 0 || (maze[game->snake_head->body.x][game->snake_head->body.y - 1] == '#' && !game->drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'O';
                    snakeMovement(0, -1, game);
                }
                break;
            case 2:
                if (game->snake_head->body.x - 1 < 0 || (maze[game->snake_head->body.x - 1][game->snake_head->body.y] == '#' && !game->drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'N';
                    snakeMovement(-1, 0, game);
                }
                break;
            case 3:
                if (game->snake_head->body.y + 1 > y - 1 || (maze[game->snake_head->body.x][game->snake_head->body.y + 1] == '#' && !game->drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'E';
                    snakeMovement(0, +1, game);
                }
                break;
            }
    } while (tmp.x == game->snake_head->body.x && tmp.y == game->snake_head->body.y);
}

void check_near_cells_S(char **maze, int x, int y, int *odds, neighbors_t cells, game_t *game)
{
    coordinates_t tmp;
    tmp.x = game->snake_head->body.x;
    tmp.y = game->snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            game->automove = 'N';
            snakeMovement(-1, 0, game);
        }
        else
            switch (*odds)
            {
            case 1:
                if (game->snake_head->body.y + 1 > y - 1 || (maze[game->snake_head->body.x][game->snake_head->body.y + 1] == '#' && !game->drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'E';
                    snakeMovement(0, +1, game);
                }
                break;
            case 2:
                if (game->snake_head->body.x + 1 > x - 1 || (maze[game->snake_head->body.x + 1][game->snake_head->body.y] == '#' && !game->drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'S';
                    snakeMovement(+1, 0, game);
                }
                break;
            case 3:
                if (game->snake_head->body.y - 1 < 0 || (maze[game->snake_head->body.x][game->snake_head->body.y - 1] == '#' && !game->drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'O';
                    snakeMovement(0, -1, game);
                }
                break;
            }
    } while (tmp.x == game->snake_head->body.x && tmp.y == game->snake_head->body.y);
}

void check_near_cells_E(char **maze, int x, int y, int *odds, neighbors_t cells, game_t *game)
{
    coordinates_t tmp;
    tmp.x = game->snake_head->body.x;
    tmp.y = game->snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            game->automove = 'O';
            snakeMovement(0, -1, game);
        }
        else
            switch (*odds)
            {
            case 1:
                if (game->snake_head->body.x - 1 < 0 || (maze[game->snake_head->body.x - 1][game->snake_head->body.y] == '#' && !game->drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'N';
                    snakeMovement(-1, 0, game);
                }
                break;
            case 2:
                if (game->snake_head->body.y + 1 > y - 1 || (maze[game->snake_head->body.x][game->snake_head->body.y + 1] == '#' && !game->drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'E';
                    snakeMovement(0, +1, game);
                }
                break;
            case 3:
                if (game->snake_head->body.x + 1 > x - 1 || (maze[game->snake_head->body.x + 1][game->snake_head->body.y] == '#' && !game->drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'S';
                    snakeMovement(+1, 0, game);
                }
                break;
            }
    } while (tmp.x == game->snake_head->body.x && tmp.y == game->snake_head->body.y);
}

void check_near_cells_O(char **maze, int x, int *odds, neighbors_t cells, game_t *game)
{
    coordinates_t tmp;
    tmp.x = game->snake_head->body.x;
    tmp.y = game->snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            game->automove = 'E';
            snakeMovement(0, +1, game);
        }
        else
            switch (*odds)
            {
            case 1:
                if (game->snake_head->body.x + 1 > x - 1 || (maze[game->snake_head->body.x + 1][game->snake_head->body.y] == '#' && !game->drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'S';
                    snakeMovement(+1, 0, game);
                }
                break;
            case 2:
                if (game->snake_head->body.y - 1 < 0 || (maze[game->snake_head->body.x][game->snake_head->body.y - 1] == '#' && !game->drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'O';
                    snakeMovement(0, -1, game);
                }
                break;
            case 3:
                if (game->snake_head->body.x - 1 < 0 || (maze[game->snake_head->body.x - 1][game->snake_head->body.y] == '#' && !game->drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    game->automove = 'N';
                    snakeMovement(-1, 0, game);
                }
                break;
            }
    } while (tmp.x == game->snake_head->body.x && tmp.y == game->snake_head->body.y);
}

void move_random(char **maze, int x, int y, path_t *path, game_t *game)
{
    int odds;
    neighbors_t cells;
    cells.cell_1 = false;
    cells.cell_2 = false;
    cells.cell_3 = false;
    odds = rand() % 3 + 1;
    snakeClear(maze, game);
    switch (toupper(game->automove))
    {
    case 'N':
        check_near_cells_N(maze, y, &odds, cells, game);
        break;
    case 'S':
        check_near_cells_S(maze, x, y, &odds, cells, game);
        break;
    case 'E':
        check_near_cells_E(maze, x, y, &odds, cells, game);
        break;
    case 'O':
        check_near_cells_O(maze, x, &odds, cells, game);
        break;
    }
    add_move(path, game);
    if (maze[game->snake_head->body.x][game->snake_head->body.y] == '#' && game->drills)
        game->drills--;
    else
        check_collectable(maze, game);
    snakePrint(maze, game);
}

void mark_path(path_t *path, char **maze, game_t *game)
{
    int row = game->entrance.x, col = game->entrance.y;
    maze[row][col] = '.';

    for (size_t i = 0; i < path->size; i++)
    {
        switch (path->moves[i])
        {
        case 'N':
            row--;
            break;
        case 'S':
            row++;
            break;
        case 'E':
            col++;
            break;
        case 'O':
            col--;
            break;
        }
        maze[row][col] = '.';
    }
}