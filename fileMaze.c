#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>



typedef struct
{
    int x;
    int y;
} coordinates;

coordinates player;
coordinates entrance, exit_maze;
char direction;
int points;
int steps;
int coins = 0, drills = 0;

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
                    player.x = entrance.x;
                    player.y = entrance.y;
                    maze[player.x][player.y] = 'o';
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
    int ctrCoin = 10, ctrPenalty = 5, ctrDrill = 2;
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

int **createNumMaze(char **maze, int x, int y) {
    int **numMaze = malloc(x * sizeof(int*));

    for (int i = 0; i < y; i++)
        numMaze[i] = malloc(y * sizeof(int*));

    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++){
            switch(maze[i][j]){
                case ' ':
                    numMaze[i][j] = 0;
                    break;
                case '#':
                    numMaze[i][j] = 1;
                    break;
                case '$':
                    numMaze[i][j] = 2;
                    break;
                case '!':
                    numMaze[i][j] = 3;
                    break;
                case 'T':
                    numMaze[i][j] = 4;
                    break;
                case 'o':
                    numMaze[i][j] = 6;
                    break;
                case '_':
                    numMaze[i][j] = 5;
                    break;
            }
        }
    }
    return numMaze;
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

void updateSnake(int **numMaze, char **maze, int playerX, int playerY){

    int identifier = numMaze[playerX][playerY];
    int tmpCoin = coins;
    int tmpX = playerX;
    int tmpY = playerY;

    while(tmpCoin > 0) {
        if(numMaze[playerX - 1][playerY] == identifier - 1){
            maze[playerX - 1][playerY] = '?';
            tmpX--;
        }
        else if(numMaze[playerX + 1][playerY] == identifier - 1) {
            maze[playerX + 1][playerY] = '?';
            tmpX++;
        }
        if(numMaze[playerX][playerY + 1] == identifier - 1){
            maze[playerX][playerY + 1] = '?';
            tmpY++;
        }
        else if(numMaze[playerX][playerY - 1] == identifier - 1){
            maze[playerX][playerY - 1] = '?';
            tmpY--;
        }
        tmpCoin--;
    }

    printf("\n%d %d \n", playerX, playerY);
    printf("\n%d %d \n", tmpX, tmpY);
    printf("\n%d \n", coins);

    if(numMaze[playerX - 1][playerY] == identifier - coins - 1){
        maze[tmpX - 1][tmpY] = ' ';   
    }
    else if(numMaze[playerX + 1][playerY] == identifier - coins - 1) {
        maze[tmpX + 1][tmpY] = ' ';
    }
    if(numMaze[playerX][playerY + 1] == identifier - coins - 1){
        maze[tmpX][tmpY + 1] = ' ';
    }
    else if(numMaze[playerX][playerY - 1] == identifier - coins - 1){
        maze[tmpX][tmpY - 1] = ' ';
    }

}

void printNumMaze(int **numMaze, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
            printf("%d ", numMaze[i][j]);
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
    if (player.x == exit_maze.x && player.y == exit_maze.y)
        return true;
    return false;
}

void move(char direction,  char **maze, int **numMaze, int identifier)
{
    switch (tolower(direction))
    {
    case 'n':
        if (numMaze[player.x - 1][player.y] != 1)
        {
            maze[player.x][player.y] = ' ';
            switch(numMaze[player.x - 1][player.y]){
                case 2: 
                    coins++;
                    break;
                case 3:
                    coins /= 2;
                    break;
                case 4:
                    drills += 3;
                    break;
            }
            player.x -= 1;
            numMaze[player.x][player.y] = identifier;

        }
        break;
    case 's':
        if (numMaze[player.x + 1][player.y] != 1)
        {
            maze[player.x][player.y] = ' ';

            switch(numMaze[player.x + 1][player.y]){
                case 2: 
                    coins++;
                    break;
                case 3:
                    coins /= 2;
                    break;
                case 4:
                    drills += 3;
                    break;
            }
            player.x += 1;
            numMaze[player.x][player.y] = identifier;
        }
        break;
    case 'e':
        if (numMaze[player.x][player.y + 1] != 1)
        {
            maze[player.x][player.y] = ' ';

            switch(numMaze[player.x][player.y + 1]){
                case 2: 
                    coins++;
                    break;
                case 3:
                    coins /= 2;
                    break;
                case 4:
                    drills += 3;
                    break;
            }
            player.y += 1;
            numMaze[player.x][player.y] = identifier;
        }
        break;
    case 'o':
        if (numMaze[player.x][player.y - 1] != 1)
        {
            maze[player.x][player.y] = ' ';
        
            switch(numMaze[player.x][player.y - 1]){
                case 2: 
                    coins++;
                    break;
                case 3:
                    coins /= 2;
                    break;
                case 4:
                    drills += 3;
                    break;
            }
            player.y -= 1;
            numMaze[player.x][player.y] = identifier;
        }
        break;
    }
    maze[player.x][player.y] = 'o';

    updateSnake(numMaze, maze, player.x, player.y);

}