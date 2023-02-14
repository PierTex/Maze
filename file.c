#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "settings.h"

coordinates player;
coordinates entrance, exit_maze;
coordinates backup;

char direction;
int points;
int steps;
int ctrCoin = 10, ctrPenalty = 5, ctrDrill = 2;

list_t *snake_head;
list_t *head;
list_t *snake_tail = NULL;

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

void resetColor()
{
    printf("\033[0m");
}

void black()
{
    printf("\033[0:30m");
}

void red()
{
    printf("\033[0:31m");
}

void green()
{
    printf("\033[0:32m");
}

void yellow()
{
    printf("\033[0:33m");
}

void blue()
{
    printf("\033[0:34m");
}

void purple()
{
    printf("\033[0:35m");
}

void cyan()
{
    printf("\033[0:36m");
}

void white()
{
    printf("\033[0:37m");
}

void create_snake_head()
{
    snake_head = (list_t *)malloc(sizeof(list_t));
    head = snake_head;
    head->next = snake_tail;
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
    new_body->next = snake_tail;
}

void snakeMovement(int x, int y)
{
    while (head->next)
    {
        head->body.x += x;
        head->body.y += y;
        head = head->next;
    }
    backup.x = head->body.x;
    backup.y = head->body.y;
    head->body.x += x;
    head->body.y += y;
}

char **createMaze(int x, int y)
{
    srand(time(NULL));

    char **maze = malloc(x * sizeof(char *));

    for (int i = 0; i < y; i++)
        maze[i] = malloc(y * sizeof(char *));

    int nWalls, pHole, ctr, ctrCol;
    int lenCorridor = ((rand() % 3) + 2); // larghezza corridoio (2 o 3 spazi)

    // creazione entrata e uscita
    entrance.x = rand() % (x - 2) + 1;
    entrance.y = 0;
    exit_maze.x = rand() % (x - 2) + 1;
    exit_maze.y = y - 1;

    // stampa bordo
    bool frame;         // controllo bordo labirinto
    bool entrance_exit; // controllo entrata o uscita
    for (int row = 0; row < x; ++row)
    {
        for (int col = 0; col < y; ++col)
        {
            frame = (row == 0 || row == x - 1 || col == 0 || col == y - 1);
            entrance_exit = ((col == entrance.y && row == entrance.x) || (col == exit_maze.y && row == exit_maze.x));
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

                    maze[head->body.x][head->body.y] = 'o';
                }
                if (col == exit_maze.y && row == exit_maze.x)
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
    coordinates coin, penalty, drill;
    bool isAvailableCoin, isAvailablePenalty, isAvailableDrill; // controllo cella
    while (ctrCoin || ctrPenalty || ctrDrill)
    {
        if (ctrCoin > 0)
        {
            do
            {
                coin.x = rand() % (x - 2) + 1;
                coin.y = rand() % (y - 2) + 1;
                isAvailableCoin = (maze[coin.x][coin.y] == ' ' && !(coin.x == entrance.x && coin.y == entrance.y) && !(coin.x == exit_maze.x && coin.y == exit_maze.y));
            } while (!isAvailableCoin);
            maze[coin.x][coin.y] = '$';
            ctrCoin--;
        }
        if (ctrPenalty > 0)
        {
            do
            {
                penalty.x = rand() % (x - 2) + 1;
                penalty.y = rand() % (y - 2) + 1;
                isAvailablePenalty = (maze[penalty.x][penalty.y] == ' ' && !(penalty.x == entrance.x && penalty.y == entrance.y) && !(penalty.x == exit_maze.x && penalty.y == exit_maze.y));
            } while (!isAvailablePenalty);
            maze[penalty.x][penalty.y] = '!';
            ctrPenalty--;
        }
        if (ctrDrill > 0)
        {
            do
            {
                drill.x = rand() % (x - 2) + 1;
                drill.y = rand() % (y - 2) + 1;
                isAvailableDrill = (maze[drill.x][drill.y] == ' ' && !(drill.x == entrance.x && drill.y == entrance.y) && !(drill.x == exit_maze.x && drill.y == exit_maze.y));
            } while (!isAvailableDrill);
            maze[drill.x][drill.x] = 'T';
            ctrDrill--;
        }
    }

    return maze;
}

void printMaze(char **maze, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
            printf("%c", maze[i][j]);
        printf("\n");
    }
}

char **inputFile(int M, int N)
{
    char *line;
    char **maze;
    line = malloc((M + 2) * sizeof(char *));
    maze = malloc(N * sizeof(char *));

    for (size_t i = 0; i < N; ++i)
        maze[i] = malloc(M * sizeof(char *));

    for (int i = 0; i < N; ++i)
    {
        fgets(line, M + 2, stdin);
        for (int j = 0; j < M; ++j)
            maze[i][j] = line[j];
    }

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

void finish()
{
    red();
    printf("You Win!!\n");
    resetColor();
}

bool checkFinish()
{
    if (head->body.x == exit_maze.x && head->body.y == exit_maze.y)
        return true;
    return false;
}

void move(char direction, char **maze)
{
    head = snake_head;
    switch (tolower(direction))
    {
    case 'n':
        if (maze[head->body.x - 1][head->body.y] != '#')
        {
            if (maze[snake_head->body.x - 1][snake_head->body.y] == '$')
            {
                snakeMovement(-1, 0);
                snakeAppend(create_body(), backup.x, backup.y);
                ctrCoin++;
            }
            else
            {
                if (maze[snake_head->body.x - 1][snake_head->body.y] == '!')
                {
                    // snakeResize();
                    ctrPenalty++;
                }
                else if (maze[snake_head->body.x - 1][snake_head->body.y] == 'T')
                    ctrDrill += 3;
                snakeMovement(-1, 0);
            }
        }
        break;
    case 's':
        if (maze[head->body.x + 1][head->body.y] != '#')
        {
            maze[head->body.x][head->body.y] = ' ';
            head->body.x += 1;
        }
        break;
    case 'e':
        if (maze[head->body.x - 1][head->body.y])
        {
            maze[head->body.x][head->body.y] = ' ';
            head->body.y += 1;
        }
        break;
    case 'o':
        if (maze[head->body.x][head->body.y - 1] != '#')
        {
            maze[head->body.x][head->body.y] = ' ';
            head->body.y -= 1;
        }
        break;
    }
    maze[snake_head->body.x][snake_head->body.y] = 'o';
    // creare funzione per aggiornamento serpente nel maze (anche una funzione clear)
}