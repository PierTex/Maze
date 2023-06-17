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

coordinates_t entrance, ending, backup;

char direction, automove;
unsigned steps = 0;
int coins, penalties, drills;

list_t *snake_head = NULL;
list_t *head;

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

int score(size_t steps)
{
    int max = 1000;
    return max + coins * 10 - steps;
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

void create_snake_head()
{
    snake_head = (list_t *)malloc(sizeof(list_t));
    if (!snake_head)
    {
        printf("\nErrore, allocamento memoria fallito\n");
        exit(EXIT_FAILURE);
    }
    head = snake_head;
    head->next = NULL;
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

void snakeAppend(list_t *new_body, int x, int y)
{
    head->next = new_body;
    new_body->body.x = x;
    new_body->body.y = y;
    new_body->next = NULL;
}

void snakeMovement(int x, int y)
{
    coordinates_t last_pos, current_pos;
    last_pos.x = head->body.x;
    last_pos.y = head->body.y;
    head->body.x += x;
    head->body.y += y;
    while (head->next)
    {
        head = head->next;
        current_pos.x = head->body.x;
        current_pos.y = head->body.y;
        head->body.x = last_pos.x;
        head->body.y = last_pos.y;
        last_pos.x = current_pos.x;
        last_pos.y = current_pos.y;
    }
    backup.x = last_pos.x;
    backup.y = last_pos.y;
    steps++;
}

void snakeShrink()
{
    int i = 0;
    list_t *ptrBackup;
    do
    {
        ptrBackup = head;
        head = head->next;
        i++;
    } while (i <= coins);
    freeSnake(head);
    head = ptrBackup;
    head->next = NULL;
}

void snakeEatingHimself()
{
    int i = 0;
    bool found = false;
    head = snake_head;
    head = head->next;
    while (head && !found)
    {
        if (snake_head->body.x == head->body.x &&
            snake_head->body.y == head->body.y)
        {
            found = true;
            coins = i;
        }
        else
            i++;
        head = head->next;
    }
    head = snake_head;
    if (found)
        snakeShrink();
}

void snakeClear(char **maze)
{
    head = snake_head;
    while (head)
    {
        maze[head->body.x][head->body.y] = ' ';
        head = head->next;
    }
    head = snake_head;
}

void snakePrint(char **maze)
{
    head = snake_head;
    maze[head->body.x][head->body.y] = '@';
    head = head->next;
    while (head)
    {
        maze[head->body.x][head->body.y] = 'o';
        head = head->next;
    }
    head = snake_head;
}

char **createMaze(int x, int y)
{
    coins = 10;
    penalties = 5;
    drills = 2;

    char **maze = malloc(x * sizeof(char *));

    for (int i = 0; i < x; i++)
        maze[i] = malloc(y * sizeof(char *));

    int nWalls, pHole, ctrCol;
    int lenCorridor = ((rand() % 3) + 2); // larghezza corridoio (2 o 3 spazi)

    // creazione entrata e uscita
    entrance.x = rand() % (x - 2) + 1;
    entrance.y = 0;
    ending.x = rand() % (x - 2) + 1;
    ending.y = y - 1;

    // stampa bordo
    bool frame;         // controllo bordo labirinto
    bool entrance_exit; // controllo entrata o uscita
    for (int row = 0; row < x; ++row)
    {
        for (int col = 0; col < y; ++col)
        {
            frame = (row == 0 || row == x - 1 || col == 0 || col == y - 1);
            entrance_exit = ((col == entrance.y && row == entrance.x) || (col == ending.y && row == ending.x));
            if (frame && !entrance_exit)
                maze[row][col] = '#';
            else
            {
                maze[row][col] = ' ';
                if (col == entrance.y && row == entrance.x)
                {
                    create_snake_head();
                    head->body.x = entrance.x;
                    head->body.y = entrance.y;

                    maze[head->body.x][head->body.y] = '@';
                }
                if (col == ending.y && row == ending.x)
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
            ctrCol = 0;
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
                ctrCol++;
            } while (!atLeast1);
            if (ctrCol > 2 && !atLeast1)
            { // se non genera un buco in 3 cicli allora lo genera obbligatoriamente in una posizione random
                k = rand() % (x - 3) + 1;
                maze[k][col] = ' ';
                maze[k + 1][col] = ' ';
                atLeast1 = true;
            }
        }
    }

    // generazione monete, penalita' e trapani
    coordinates_t coin, penalty, drill;
    bool isAvailableCoin, isAvailablePenalty, isAvailableDrill; // controllo cella
    while (coins || penalties || drills)
    {
        if (coins > 0)
        {
            do
            {
                coin.x = rand() % (x - 2) + 1;
                coin.y = rand() % (y - 2) + 1;
                isAvailableCoin = (maze[coin.x][coin.y] == ' ' && !(coin.x == entrance.x && coin.y == entrance.y) && !(coin.x == ending.x && coin.y == ending.y));
            } while (!isAvailableCoin);
            maze[coin.x][coin.y] = '$';
            coins--;
        }
        if (penalties > 0)
        {
            do
            {
                penalty.x = rand() % (x - 2) + 1;
                penalty.y = rand() % (y - 2) + 1;
                isAvailablePenalty = (maze[penalty.x][penalty.y] == ' ' && !(penalty.x == entrance.x && penalty.y == entrance.y) && !(penalty.x == ending.x && penalty.y == ending.y));
            } while (!isAvailablePenalty);
            maze[penalty.x][penalty.y] = '!';
            penalties--;
        }
        if (drills > 0)
        {
            do
            {
                drill.x = rand() % (x - 2) + 1;
                drill.y = rand() % (y - 2) + 1;
                isAvailableDrill = (maze[drill.x][drill.y] == ' ' && !(drill.x == entrance.x && drill.y == entrance.y) && !(drill.x == ending.x && drill.y == ending.y));
            } while (!isAvailableDrill);
            maze[drill.x][drill.x] = 'T';
            drills--;
        }
    }

    return maze;
}

char **copy_matrix(int x, int y, char **maze_to, char **maze_from)
{
    for (int i = 0; i < x; ++i)
        for (int j = 0; j < y; ++j)
            maze_to[i][j] = maze_from[i][j];

    freeSnake(snake_head);
    snake_head = NULL;

    return maze_to;
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

char insertMove()
{
    char direction;

    printf("\nMonete:\t\t%d\nPenalita':\t%d\nTrapani:\t%d\n\n", coins, penalties, drills);
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

void finish(char **maze, int x)
{
    freeSnake(snake_head);
    freeMaze(maze, x);
    printf("\nHai vinto!!!\nPunteggio: %d\n", score(steps));
    new_line();
}

bool checkFinish()
{
    if (head->body.x == ending.x && head->body.y == ending.y)
        return true;
    return false;
}

void check_collectable(char **maze)
{
    if (maze[snake_head->body.x][snake_head->body.y] == '$')
    {
        snakeAppend(create_body(), backup.x, backup.y);
        coins++;
    }
    else if (maze[snake_head->body.x][snake_head->body.y] == '!')
    {
        if (coins)
        {
            head = snake_head;
            coins /= 2;
            snakeShrink();
        }
        penalties++;
    }
    else if (maze[snake_head->body.x][snake_head->body.y] == 'T')
        drills += 3;
    else
    {
        if (coins > 1)
            snakeEatingHimself();
    }
}

void move(char direction, char **maze, int x, int y)
{
    bool supreme_wall;
    head = snake_head;
    snakeClear(maze);
    switch (tolower(direction))
    {
    case 'n':
        supreme_wall = snake_head->body.x - 1 < 0;
        if (!supreme_wall && (maze[snake_head->body.x - 1][snake_head->body.y] != '#' || drills))
            snakeMovement(-1, 0);
        break;
    case 's':
        supreme_wall = snake_head->body.x + 1 > x - 1;
        if (!supreme_wall && (maze[snake_head->body.x + 1][snake_head->body.y] != '#' || drills))
            snakeMovement(+1, 0);
        break;
    case 'e':
        supreme_wall = snake_head->body.y + 1 > y - 1;
        if (!supreme_wall && (maze[snake_head->body.x][snake_head->body.y + 1] != '#' || drills))
            snakeMovement(0, +1);
        break;
    case 'o':
        supreme_wall = snake_head->body.y - 1 < 0;
        if (!supreme_wall && (maze[snake_head->body.x][snake_head->body.y - 1] != '#' || drills))
            snakeMovement(0, -1);
        break;
    }
    if (maze[snake_head->body.x][snake_head->body.y] == '#' && drills)
        drills--;
    else
        check_collectable(maze);
    snakePrint(maze);
}

// Funzioni AI

void free_path(path_t *path)
{
    free(path->moves);
    path->capacity = 0;
    path->size = 0;
}

void init_path(path_t *path)
{
    coins = 0;
    penalties = 0;
    drills = 0;
    path->moves = (char *)malloc(INIT_CAPACITY * sizeof(char) + 1);
    path->capacity = INIT_CAPACITY;
    path->size = 0;
}

void add_move(path_t *path)
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
    path->moves[path->size++] = automove;
}

void print_path(path_t *path)
{
    for (size_t i = 0; i < path->size; i++)
        printf("%c", path->moves[i]);
    printf("\n");
}

void finish_AI(char **maze, int x, path_t *path)
{
    printf("\nPunteggio: %d\nPercorso effettuato: ", score(path->size));
    path->moves[path->size] = '\0';
    print_path(path);
    new_line();
    free_path(path);
    freeSnake(snake_head);
    freeMaze(maze, x);
}

void find_entrance_exit(char **maze, int x, int y)
{
    coins = 0;
    penalties = 0;
    drills = 0;

    create_snake_head();

    for (int row = 0; row < x; ++row)
        for (int col = 0; col < y; ++col)
        {
            if (maze[row][col] == 'o')
            {
                entrance.x = row;
                entrance.y = col;
                if (entrance.x == 0)
                    automove = 'S';
                else if (entrance.x == x - 1)
                    automove = 'N';
                else if (entrance.y == 0)
                    automove = 'E';
                else if (entrance.y == y - 1)
                    automove = 'O';
                snake_head->body.x = row;
                snake_head->body.y = col;
            }
            if (maze[row][col] == '_')
            {
                ending.x = row;
                ending.y = col;
            }
        }
}

void move_right_hand(char **maze, int x, int y, path_t *path)
{
    head = snake_head;
    switch (toupper(automove))
    {
    case 'N':
        if (!(snake_head->body.y + 1 > y - 1) &&
            (maze[snake_head->body.x][snake_head->body.y + 1] != '#' || drills))
        {
            automove = 'E';
            snakeMovement(0, +1);
        }
        else if (!(snake_head->body.x - 1 < 0) &&
                 (maze[snake_head->body.x - 1][snake_head->body.y] != '#' || drills))
        {
            snakeMovement(-1, 0);
        }
        else if (!(snake_head->body.y - 1 < 0) &&
                 (maze[snake_head->body.x][snake_head->body.y - 1] != '#' || drills))
        {
            automove = 'O';
            snakeMovement(0, -1);
        }
        else
        {
            automove = 'S';
            snakeMovement(+1, 0);
        }
        break;
    case 'S':
        if (!(snake_head->body.y - 1 < 0) &&
            (maze[snake_head->body.x][snake_head->body.y - 1] != '#' || drills))
        {
            automove = 'O';
            snakeMovement(0, -1);
        }
        else if (!(snake_head->body.x + 1 > x - 1) &&
                 (maze[snake_head->body.x + 1][snake_head->body.y] != '#' || drills))
        {
            snakeMovement(+1, 0);
        }
        else if (!(snake_head->body.y + 1 > y - 1) &&
                 (maze[snake_head->body.x][snake_head->body.y + 1] != '#' || drills))
        {
            automove = 'E';
            snakeMovement(0, +1);
        }
        else
        {
            automove = 'N';
            snakeMovement(-1, 0);
        }
        break;
    case 'E':
        if (!(snake_head->body.x + 1 > x - 1) &&
            (maze[snake_head->body.x + 1][snake_head->body.y] != '#' || drills))
        {
            automove = 'S';
            snakeMovement(+1, 0);
        }
        else if (!(snake_head->body.y + 1 > y - 1) &&
                 (maze[snake_head->body.x][snake_head->body.y + 1] != '#' || drills))
        {
            snakeMovement(0, +1);
        }
        else if (!(snake_head->body.x - 1 < 0) &&
                 (maze[snake_head->body.x - 1][snake_head->body.y] != '#' || drills))
        {
            automove = 'N';
            snakeMovement(-1, 0);
        }
        else
        {
            automove = 'O';
            snakeMovement(0, -1);
        }
        break;
    case 'O':
        if (!(snake_head->body.x - 1 < 0) &&
            (maze[snake_head->body.x - 1][snake_head->body.y] != '#' || drills))
        {
            automove = 'N';
            snakeMovement(-1, 0);
        }
        else if (!(snake_head->body.y - 1 < 0) &&
                 (maze[snake_head->body.x][snake_head->body.y - 1] != '#' || drills))
        {
            snakeMovement(0, -1);
        }
        else if (!(snake_head->body.x + 1 > x - 1) &&
                 (maze[snake_head->body.x + 1][snake_head->body.y] != '#' || drills))
        {
            automove = 'S';
            snakeMovement(+1, 0);
        }
        else
        {
            automove = 'E';
            snakeMovement(0, +1);
        }
        break;
    }
    add_move(path);
    if (maze[snake_head->body.x][snake_head->body.y] == '#' && drills)
    {
        if (automove == 'N')
            automove = 'O';
        else if (automove == 'S')
            automove = 'E';
        else if (automove == 'E')
            automove = 'N';
        else if (automove == 'O')
            automove = 'S';
        drills--;
    }
    else
        check_collectable(maze);
}

void check_near_cells_N(char **maze, int y, int *odds, neighbors_t cells)
{
    coordinates_t tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            automove = 'S';
            snakeMovement(0, +1);
        }
        else
            switch (*odds)
            {
            case 1:
                if (snake_head->body.y - 1 < 0 || (maze[snake_head->body.x][snake_head->body.y - 1] == '#' && !drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'O';
                    snakeMovement(0, -1);
                }
                break;
            case 2:
                if (snake_head->body.x - 1 < 0 || (maze[snake_head->body.x - 1][snake_head->body.y] == '#' && !drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'N';
                    snakeMovement(-1, 0);
                }
                break;
            case 3:
                if (snake_head->body.y + 1 > y - 1 || (maze[snake_head->body.x][snake_head->body.y + 1] == '#' && !drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'E';
                    snakeMovement(0, +1);
                }
                break;
            }
    } while (tmp.x == snake_head->body.x && tmp.y == snake_head->body.y);
}

void check_near_cells_S(char **maze, int x, int y, int *odds, neighbors_t cells)
{
    coordinates_t tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            automove = 'N';
            snakeMovement(-1, 0);
        }
        else
            switch (*odds)
            {
            case 1:
                if (snake_head->body.y + 1 > y - 1 || (maze[snake_head->body.x][snake_head->body.y + 1] == '#' && !drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'E';
                    snakeMovement(0, +1);
                }
                break;
            case 2:
                if (snake_head->body.x + 1 > x - 1 || (maze[snake_head->body.x + 1][snake_head->body.y] == '#' && !drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'S';
                    snakeMovement(+1, 0);
                }
                break;
            case 3:
                if (snake_head->body.y - 1 < 0 || (maze[snake_head->body.x][snake_head->body.y - 1] == '#' && !drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'O';
                    snakeMovement(0, -1);
                }
                break;
            }
    } while (tmp.x == snake_head->body.x && tmp.y == snake_head->body.y);
}

void check_near_cells_E(char **maze, int x, int y, int *odds, neighbors_t cells)
{
    coordinates_t tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            automove = 'O';
            snakeMovement(0, -1);
        }
        else
            switch (*odds)
            {
            case 1:
                if (snake_head->body.x - 1 < 0 || (maze[snake_head->body.x - 1][snake_head->body.y] == '#' && !drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'N';
                    snakeMovement(-1, 0);
                }
                break;
            case 2:
                if (snake_head->body.y + 1 > y - 1 || (maze[snake_head->body.x][snake_head->body.y + 1] == '#' && !drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'E';
                    snakeMovement(0, +1);
                }
                break;
            case 3:
                if (snake_head->body.x + 1 > x - 1 || (maze[snake_head->body.x + 1][snake_head->body.y] == '#' && !drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'S';
                    snakeMovement(+1, 0);
                }
                break;
            }
    } while (tmp.x == snake_head->body.x && tmp.y == snake_head->body.y);
}

void check_near_cells_O(char **maze, int x, int *odds, neighbors_t cells)
{
    coordinates_t tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    do
    {
        if (cells.cell_1 && cells.cell_2 && cells.cell_3)
        {
            automove = 'E';
            snakeMovement(0, +1);
        }
        else
            switch (*odds)
            {
            case 1:
                if (snake_head->body.x + 1 > x - 1 || (maze[snake_head->body.x + 1][snake_head->body.y] == '#' && !drills))
                {
                    cells.cell_1 = true;
                    while (*odds == 1)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'S';
                    snakeMovement(+1, 0);
                }
                break;
            case 2:
                if (snake_head->body.y - 1 < 0 || (maze[snake_head->body.x][snake_head->body.y - 1] == '#' && !drills))
                {
                    cells.cell_2 = true;
                    while (*odds == 2)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'O';
                    snakeMovement(0, -1);
                }
                break;
            case 3:
                if (snake_head->body.x - 1 < 0 || (maze[snake_head->body.x - 1][snake_head->body.y] == '#' && !drills))
                {
                    cells.cell_3 = true;
                    while (*odds == 3)
                        *odds = rand() % 3 + 1;
                }
                else
                {
                    automove = 'N';
                    snakeMovement(-1, 0);
                }
                break;
            }
    } while (tmp.x == snake_head->body.x && tmp.y == snake_head->body.y);
}

void move_random(char **maze, int x, int y, path_t *path)
{
    int odds;
    neighbors_t cells;
    cells.cell_1 = false;
    cells.cell_2 = false;
    cells.cell_3 = false;
    odds = rand() % 3 + 1;
    snakeClear(maze);
    switch (toupper(automove))
    {
    case 'N':
        check_near_cells_N(maze, y, &odds, cells);
        break;
    case 'S':
        check_near_cells_S(maze, x, y, &odds, cells);
        break;
    case 'E':
        check_near_cells_E(maze, x, y, &odds, cells);
        break;
    case 'O':
        check_near_cells_O(maze, x, &odds, cells);
        break;
    }
    add_move(path);
    if (maze[snake_head->body.x][snake_head->body.y] == '#' && drills)
        drills--;
    else
        check_collectable(maze);
    snakePrint(maze);
}

void mark_path(path_t *path, char **maze)
{
    int row = entrance.x, col = entrance.y;
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
