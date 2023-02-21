#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "settings.h"

coordinates player;
coordinates entrance, ending;
coordinates backup;

char direction;
int points;
int steps = 0;
int coins = 10, penalties = 5, drills = 2;

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

void new_line()
{
#ifdef __APPLE__
    printf("\n");
#endif

#ifdef __linux__
    printf("\n");
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

int score()
{
    int max = 1000;
    return max + coins * 10 - steps;
}

void freeSnake(list_t *l)
{
    list_t *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void freeMaze(char **maze, int x)
{
    for (size_t i = 0; i < x; ++i)
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
    coordinates last_pos, current_pos;
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
    head->next = snake_tail;
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
    char **maze = malloc(x * sizeof(char *));

    for (int i = 0; i < x; i++)
        maze[i] = malloc(y * sizeof(char *));

    int nWalls, pHole, ctr, ctrCol;
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
    coordinates coin, penalty, drill;
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

    char bin;
    scanf("%c", &bin);

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
    red();
    printf("\nHai vinto!!!\nPunteggio: %d\n", score());
    new_line();
    resetColor();
}

bool checkFinish()
{
    if (head->body.x == ending.x && head->body.y == ending.y)
        return true;
    return false;
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
        if (!supreme_wall && maze[snake_head->body.x - 1][snake_head->body.y] != '#')
        {
            if (maze[snake_head->body.x - 1][snake_head->body.y] == '$')
            {
                snakeMovement(-1, 0);
                snakeAppend(create_body(), backup.x, backup.y);
                coins++;
            }
            else if (maze[snake_head->body.x - 1][snake_head->body.y] == '!')
            {
                snakeMovement(-1, 0);
                if (coins)
                {
                    head = snake_head;
                    coins /= 2;
                    snakeShrink();
                }
                penalties++;
            }
            else if (maze[snake_head->body.x - 1][snake_head->body.y] == 'T')
            {
                drills += 3;
                snakeMovement(-1, 0);
            }
            else
            {
                snakeMovement(-1, 0);
                if (coins > 1)
                    snakeEatingHimself();
            }
        }
        else if (!supreme_wall && drills)
        {
            snakeMovement(-1, 0);
            drills--;
        }
        break;
    case 's':
        supreme_wall = snake_head->body.x + 1 > x - 1;
        if (!supreme_wall && maze[snake_head->body.x + 1][snake_head->body.y] != '#')
        {
            if (maze[snake_head->body.x + 1][snake_head->body.y] == '$')
            {
                snakeMovement(+1, 0);
                snakeAppend(create_body(), backup.x, backup.y);
                coins++;
            }
            else if (maze[snake_head->body.x + 1][snake_head->body.y] == '!')
            {
                snakeMovement(+1, 0);
                if (coins)
                {
                    head = snake_head;
                    coins /= 2;
                    snakeShrink();
                }
                penalties++;
            }
            else if (maze[snake_head->body.x + 1][snake_head->body.y] == 'T')
            {
                drills += 3;
                snakeMovement(+1, 0);
            }
            else
            {
                snakeMovement(+1, 0);
                if (coins > 1)
                    snakeEatingHimself();
            }
        }
        else if (!supreme_wall && drills)
        {
            snakeMovement(+1, 0);
            drills--;
        }
        break;
    case 'e':
        supreme_wall = snake_head->body.y + 1 > y - 1;
        if (!supreme_wall && maze[snake_head->body.x][snake_head->body.y + 1] != '#')
        {
            if (maze[snake_head->body.x][snake_head->body.y + 1] == '$')
            {
                snakeMovement(0, +1);
                snakeAppend(create_body(), backup.x, backup.y);
                coins++;
            }
            else if (maze[snake_head->body.x][snake_head->body.y + 1] == '!')
            {
                snakeMovement(0, +1);
                if (coins)
                {
                    head = snake_head;
                    coins /= 2;
                    snakeShrink();
                }
                penalties++;
            }
            else if (maze[snake_head->body.x][snake_head->body.y + 1] == 'T')
            {
                drills += 3;
                snakeMovement(0, +1);
            }
            else
            {
                snakeMovement(0, +1);
                if (coins > 1)
                    snakeEatingHimself();
            }
        }
        else if (!supreme_wall && drills)
        {
            snakeMovement(0, +1);
            drills--;
        }
        break;
    case 'o':
        supreme_wall = snake_head->body.y - 1 < 0;
        if (!supreme_wall && maze[snake_head->body.x][snake_head->body.y - 1] != '#')
        {
            if (maze[snake_head->body.x][snake_head->body.y - 1] == '$')
            {
                snakeMovement(0, -1);
                snakeAppend(create_body(), backup.x, backup.y);
                coins++;
            }
            else if (maze[snake_head->body.x][snake_head->body.y - 1] == '!')
            {
                snakeMovement(0, -1);
                if (coins)
                {
                    head = snake_head;
                    coins /= 2;
                    snakeShrink();
                }
                penalties++;
            }
            else if (maze[snake_head->body.x][snake_head->body.y - 1] == 'T')
            {
                drills += 3;
                snakeMovement(0, -1);
            }
            else
            {
                snakeMovement(0, -1);
                if (coins > 1)
                    snakeEatingHimself();
            }
        }
        else if (!supreme_wall && drills)
        {
            snakeMovement(0, -1);
            drills--;
        }
        break;
    }
    snakePrint(maze);
}