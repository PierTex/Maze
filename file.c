#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Node{

    int x;
    int y;
    struct Node *next;
} list_t;

typedef struct Exit{
    int x;
    int y;
} coordinate;


int playerX, playerY;
coordinate exit;
char direction;
int points;
int steps;

void resetColor() {
    printf("\033[0m");
}

void black() {
    printf("\033[0:30m");
}

void red() {
    printf("\033[0:31m");
}

void green() {
    printf("\033[0:32m");
}

void yellow() {
    printf("\033[0:33m");
}

void blue() {
    printf("\033[0:34m");
}

void purple() {
    printf("\033[0:35m");
}

void cyan() {
    printf("\033[0:36m");
}

void white() {
    printf("\033[0:37m");
}

char **createMaze(int x, int y) {

    char **maze = malloc(x*sizeof(char*));
    
    for(int i = 0; i < y; i++) {
        maze[i] = malloc(y*sizeof(char*));
    }

    int nWalls, pHole, ctr, ctrCol;
    int lenCorridor = ((rand() % 3) + 2); // larghezza corridoio (2 o 3 spazi)
    // creazione entrata e uscita
    int entrance = rand() % (x - 2) + 1;
    int x_exit = rand() % (x - 2) + 1;
    // stampa bordo
    bool frame;         // controllo bordo labirinto
    bool entrance_exit; // controllo entrata o uscita
    for (int row = 0; row < x; ++row)
    {
        for (int col = 0; col < y; ++col)
        {
            frame = (row == 0 || row == x - 1 || col == 0 || col == y - 1);
            entrance_exit = ((col == 0 && row == entrance) || (col == y - 1 && row == x_exit));
            if (frame && !entrance_exit)
                maze[row][col] = '#';
            else{
                maze[row][col] = ' ';
                if(col == 0 && row == entrance){
                    playerX = entrance;
                    playerY = col;
                    maze[playerX][playerY] = 'o';
                }
                if(col == y-1 && row == x_exit) {
                    maze[row][col] = '_';
                    exit.x = row;
                    exit.y = col;

                }
                
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
            {
                maze[row][col] = '#';
            }
        }
        if (col >= y - 7 && col <= y - 4)
        {
            lenCorridor = 2;
            for (int row = 1; row < x - 1; ++row)
            {
                maze[row][col] = '#';
            }
        }
    }

    // creazione buchi per colonna
    int k;
    bool check;    // posso creare un buco?
    bool atLeast1; // sono presenti buchi?
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
            { // se non genera un buco in 3 cicli allora lo genera obbligatoriamente random
                k = rand() % (x - 3) + 1;
                maze[k][col] = ' ';
                maze[k + 1][col] = ' ';
                atLeast1 = true;
            }
        }
    }

    int xCoin, yCoin;
    int xPenalty, yPenalty;
    int xDrill, yDrill;
    int ctrCoin = 10, ctrPenalty = 5, ctrDrill = 2;
    bool isAvailableCoin, isAvailablePenalty, isAvailableDrill; // controllo cella
    while (ctrCoin || ctrPenalty || ctrDrill)
    {
        if (ctrCoin > 0)
        {
            do
            {
                xCoin = rand() % (x - 2) + 1;
                yCoin = rand() % (y - 2) + 1;
                isAvailableCoin = (maze[xCoin][yCoin] == ' ' && !(xCoin == entrance && yCoin == 0) && !(xCoin == x_exit && yCoin == y - 1));
            } while (!isAvailableCoin);
            maze[xCoin][yCoin] = '$';
            ctrCoin--;
        }
        if (ctrPenalty > 0)
        {
            do
            {
                xPenalty = rand() % (x - 2) + 1;
                yPenalty = rand() % (y - 2) + 1;
                isAvailablePenalty = (maze[xPenalty][yPenalty] == ' ' && !(xPenalty == entrance && yPenalty == 0) && !(xPenalty == x_exit && yPenalty == y - 1));
            } while (!isAvailablePenalty);
            maze[xPenalty][yPenalty] = '!';
            ctrPenalty--;
        }
        if (ctrDrill > 0)
        {
            do
            {
                xDrill = rand() % (x - 2) + 1;
                yDrill = rand() % (y - 2) + 1;
                isAvailableDrill = (maze[xDrill][yDrill] == ' ' && !(xDrill == entrance && yDrill == 0) && !(xDrill == x_exit && yDrill == y - 1));
            } while (!isAvailableDrill);
            maze[xDrill][xDrill] = 'T';
            ctrDrill--;
        }
    }

    return maze;
}

void printMaze(char **maze, int x, int y) {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}

char **inputFile(int M, int N)
{
    char *line;
    char **maze;
    line = malloc((M + 2) * sizeof(char*));
    maze = malloc(N * sizeof(char*));

    for (size_t i = 0; i < N; ++i)
    {
        maze[i] = malloc(M * sizeof(char*));
    }

    for (int i = 0; i < N; ++i)
    {
        fgets(line, M + 2, stdin);
        for (int j = 0; j < M; ++j)
            maze[i][j] = line[j];
    }

    return maze;

}

bool checkDigitDirection(char direction) {

    if(tolower(direction) == 'n' || tolower(direction) == 's' || tolower(direction) == 'e' || tolower(direction) == 'o') {
        return true;
    }
    return false;
}

char insertMove(){

    char direction;

    scanf("%c", &direction);
    fflush(stdin);

    while(!checkDigitDirection(direction)) {
        scanf("%c", &direction);
        fflush(stdin);
    }

    return direction;
}

void finish(){
    red();
    printf("You Win!!\n");
    resetColor();
}

bool checkFinish() {
    if(playerX == exit.x && playerY == exit.y) {
        return true;
    }
    return false;
}


void move(char direction, char **maze) {

    switch(tolower(direction)) {
        case 'n':
            if(maze[playerX - 1][playerY] != '#') {
                maze[playerX][playerY] = ' ';
                playerX -= 1;
            }
            break;
        case 's':
            if(maze[playerX + 1][playerY] != '#') {
                maze[playerX][playerY] = ' ';
                playerX += 1;
            }
            break;
        case 'e':
            if(maze[playerX - 1][playerY]) {
                maze[playerX][playerY] = ' ';
                playerY += 1;
            }
            break;
        case 'o':
            if(maze[playerX][playerY - 1] != '#') {
                maze[playerX][playerY] = ' ';
                playerY -= 1;
            }
            break;
    }
}








