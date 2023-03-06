#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "settings.h"

#define INIT_CAPACITY 10
#define GROWTH_FACTOR 2

typedef struct
{
    bool cell_1;
    bool cell_2;
    bool cell_3;
} neighbors_t;

coordinates entrance,
    ending, backup;

char direction, automove;
unsigned steps = 0;
int coins, penalties, drills;

list_t *snake_head;
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

// Calcolo punteggio finale
int score(size_t steps)
{
    int max = 1000;
    return max + coins * 10 - steps;
}

// Libera le celle di memoria dall'allocazione iniziale (a partire dal nodo dato come parametro)
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

// Libera TUTTE le celle di memoria occupate per il maze
void freeMaze(char **maze, int x)
{
    for (int i = 0; i < x; ++i)
        free(maze[i]);
    free(maze);
}

// Crea la testa iniziale del serpente
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

// Crea il nodo (pezzo del corpo del serpente) che andrà utilizzato nella funzione append
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

// Accoda il nodo creato sopra nel caso ci si imbatta nella moneta
void snakeAppend(list_t *new_body, int x, int y)
{
    head->next = new_body;
    new_body->body.x = x;
    new_body->body.y = y;
    new_body->next = NULL;
}

// Opera su tutta la lista, muove la testa nella direzione desiderata e sovrascrive ogni nodo con la posizione del nodo precedente
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
}

// Riduce la dimensione del serpente nel caso si imbatta su una penalità o su se stesso
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

// Controlla se la posizione corrente della testa corrisponda alle coordinate di una parte del corpo, se sì allora verrà chiamata la funzione shrink altrimenti nulla
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

// Sovrascrive i caratteri del maze togliendo il serpente (per poi stampare quello con le posizioni modificate tramite snakePrint)
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

// Stampa il serpente con le posizioni modificate
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

// Crea il labirinto di dimensioni x e y con entrata, uscita e corridoi completamente random
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

// Stampa su schermo del labirinito
void printMaze(char **maze, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
            printf("%c", maze[i][j]);
        printf("\n");
    }
}

// Funzione per immettere il labirinto scelto dall'utente
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

// La mossa presa in input è accettabile?
bool checkDigitDirection(char direction)
{
    if (tolower(direction) == 'n' ||
        tolower(direction) == 's' ||
        tolower(direction) == 'e' ||
        tolower(direction) == 'o')
        return true;
    return false;
}

// Inserimento della mossa e output dei raccoglibili presi fino a quel momento
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

// Operazioni di conclusione partita
void finish(char **maze, int x)
{
    freeSnake(snake_head);
    freeMaze(maze, x);
    red();
    printf("\nHai vinto!!!\nPunteggio: %d\n", score(steps));
    resetColor();
    new_line();
}

// Controlla che le coordinate della testa corrispondano a quelle dell'uscita
bool checkFinish()
{
    if (head->body.x == ending.x && head->body.y == ending.y)
        return true;
    return false;
}

// Controlla che nella cella della testa del serpente ci sia o meno un elemento raccoglibile (prima di sovrascriverlo con '.' nel caso AI sempre a destra)
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

// Operazioni per i controlli e l'eventuale movimento del serpente
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
    path->moves = (char *)malloc(INIT_CAPACITY * sizeof(char) + 1);
    path->capacity = INIT_CAPACITY;
    path->size = 0;
}

// Riempie il percorso
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

// Stampa percorso
void print_path(path_t *path)
{
    for (size_t i = 0; i < path->size; i++)
        printf("%c", path->moves[i]);
    printf("\n");
}

// Operazioni di conclusione partita AI
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

// Trova l'entrata e l'uscita del maze dato in input dall'utente
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

// funzione "move" ma utilizzata nell'AI sempre a destra
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
    maze[snake_head->body.x][snake_head->body.y] = '.';
}

void check_near_cells_N(char **maze, int y, int *cell)
{
    coordinates tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    do
    {
        if (cell1_visited && cell2_visited && cell3_visited)
        {
            automove = 'S';
            snakeMovement(0, +1);
        }
        else
            switch (*cell)
            {
            case 1:
                if (snake_head->body.y - 1 < 0 || (maze[snake_head->body.x][snake_head->body.y - 1] == '#' && !drills))
                {
                    cell1_visited = true;
                    while (*cell == 1)
                        *cell = rand() % 3 + 1;
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
                    cell2_visited = true;
                    while (*cell == 2)
                        *cell = rand() % 3 + 1;
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
                    cell3_visited = true;
                    while (*cell == 3)
                        *cell = rand() % 3 + 1;
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

void check_near_cells_S(char **maze, int x, int y, int *cell)
{
    coordinates tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    bool cell1_visited = false;
    bool cell2_visited = false;
    bool cell3_visited = false;
    do
    {
        if (cell1_visited && cell2_visited && cell3_visited)
        {
            automove = 'N';
            snakeMovement(-1, 0);
        }
        else
            switch (*cell)
            {
            case 1:
                if (snake_head->body.y + 1 > y - 1 || (maze[snake_head->body.x][snake_head->body.y + 1] == '#' && !drills))
                {
                    cell1_visited = true;
                    while (*cell == 1)
                        *cell = rand() % 3 + 1;
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
                    cell2_visited = true;
                    while (*cell == 2)
                        *cell = rand() % 3 + 1;
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
                    cell3_visited = true;
                    while (*cell == 3)
                        *cell = rand() % 3 + 1;
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

void check_near_cells_E(char **maze, int x, int y, int *cell)
{
    coordinates tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    bool cell1_visited = false;
    bool cell2_visited = false;
    bool cell3_visited = false;
    do
    {
        if (cell1_visited && cell2_visited && cell3_visited)
        {
            automove = 'O';
            snakeMovement(0, -1);
        }
        else
            switch (*cell)
            {
            case 1:
                if (snake_head->body.x - 1 < 0 || (maze[snake_head->body.x - 1][snake_head->body.y] == '#' && !drills))
                {
                    cell1_visited = true;
                    while (*cell == 1)
                        *cell = rand() % 3 + 1;
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
                    cell2_visited = true;
                    while (*cell == 2)
                        *cell = rand() % 3 + 1;
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
                    cell3_visited = true;
                    while (*cell == 3)
                        *cell = rand() % 3 + 1;
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

void check_near_cells_O(char **maze, int x, int *cell)
{
    coordinates tmp;
    tmp.x = snake_head->body.x;
    tmp.y = snake_head->body.y;
    bool cell1_visited = false;
    bool cell2_visited = false;
    bool cell3_visited = false;
    do
    {
        if (cell1_visited && cell2_visited && cell3_visited)
        {
            automove = 'E';
            snakeMovement(0, +1);
        }
        else
            switch (*cell)
            {
            case 1:
                if (snake_head->body.x + 1 > x - 1 || (maze[snake_head->body.x + 1][snake_head->body.y] == '#' && !drills))
                {
                    cell1_visited = true;
                    while (*cell == 1)
                        *cell = rand() % 3 + 1;
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
                    cell2_visited = true;
                    while (*cell == 2)
                        *cell = rand() % 3 + 1;
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
                    cell3_visited = true;
                    while (*cell == 3)
                        *cell = rand() % 3 + 1;
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

// funzione "move" ma utilizzata nell'AI random
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
        check_near_cells_N(maze, y, &odds);
        break;
    case 'S':
        check_near_cells_S(maze, x, y, &odds);
        break;
    case 'E':
        check_near_cells_E(maze, x, y, &odds);
        break;
    case 'O':
        check_near_cells_O(maze, x, &odds);
        break;
    }
    // print_path(steps, path);
    add_move(path);
    if (maze[snake_head->body.x][snake_head->body.y] == '#' && drills)
        drills--;
    else
        check_collectable(maze);
    // maze[snake_head->body.x][snake_head->body.y] = '.';
    snakePrint(maze);
    printf("\n");
    printMaze(maze, x, y);
    print_path(path);
    printf("automove: %c\ncoins: %d\tpenalties: %d\tdrills: %d\n", automove, coins, penalties, drills);
}
