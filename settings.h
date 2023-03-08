#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int x;
    int y;
} coordinates_t;

typedef struct Node
{
    coordinates_t body;
    struct Node *next;
} list_t;

typedef struct
{
    char *moves;
    size_t capacity;
    size_t size;
} path_t;

typedef struct
{
    int current;
    int best;
} score_t;

typedef struct
{
    bool cell_1;
    bool cell_2;
    bool cell_3;
} neighbors_t;

// FUNZIONI GENERICHE

// Pulisce lo schermo dalla porzione di testo scritta precedentemente
void refresh();

// Gestisce la possibilita' di scrivere una newline oppure no in base al sistema operativo
void new_line();

// Calcolo punteggio finale
int score(size_t steps);

// Libera le celle di memoria dall'allocazione iniziale (a partire dal nodo dato come parametro)
void freeSnake(list_t *l);

// Libera TUTTE le celle di memoria occupate dal maze scelto come parametro (x e' il numero di righe della matrice)
void freeMaze(char **maze, int x);

// Alloca lo spazio necessario per la generazione della testa dello Snake
void create_snake_head();

// Crea il nodo (pezzo del corpo dello Snake) che andrà utilizzato nella funzione append
list_t *create_body();

// Accoda il nodo creato sopra nel caso ci si imbatta nella moneta
void snakeAppend(list_t *new_body, int x, int y);

// Opera su tutta la lista, muove la testa nella direzione desiderata e sovrascrive ogni nodo con la posizione del nodo precedente
void snakeMovement(int x, int y);

// Riduce la dimensione dello Snake nel caso si imbatta su una penalità o su se stesso
void snakeShrink();

// Controlla se la posizione corrente della testa corrisponda alle coordinate di una parte del corpo, se sì allora verrà chiamata la funzione shrink altrimenti nulla
void snakeEatingHimself();

// Sovrascrive i caratteri del maze togliendo lo Snake (per poi stampare quello con le posizioni modificate tramite snakePrint)
void snakeClear(char **maze);

// Stampa lo Snake con le posizioni modificate
void snakePrint(char **maze);

// Crea il labirinto di dimensioni x e y con entrata, uscita e corridoi completamente random
char **createMaze(int x, int y);

// Funzione che copia il contenuto di una matrice (maze_from) in un'altra (maze_to), utilizzata nella AI Random
char **copy_matrix(int x, int y, char **maze_to, char **maze_from);

// Stampa su schermo del labirinito
void printMaze(char **maze, int x, int y);

// Funzione per immettere il labirinto scelto dall'utente
char **inputFile(int M, int N);

// La mossa presa in input è accettabile?
bool checkDigitDirection(char direction);

// Inserimento della mossa e output dei raccoglibili presi fino a quel momento
char insertMove();

// Controlla che le coordinate della testa corrispondano a quelle dell'uscita
bool checkFinish();

// Operazioni di conclusione partita
void finish(char **maze, int x);

// Controlla che nella cella della testa dello Snake ci sia o meno un elemento raccoglibile (prima di sovrascriverlo con '.' nel caso AI sempre a destra)
void check_collectable(char **maze);

// Operazioni per i controlli e l'eventuale movimento del serpente
void move(char direction, char **maze, int x, int y);

// Funzioni specifiche per le strategie AI

// Libera il puntatore del percorso dalla memoria allocata precedentemente e azzera le sotto-variabili 'capacity' e 'size'
void free_path(path_t *path);

// Inizializza la variabile di tipo path_t allocando memoria iniziale a 'moves', impostando una 'capacity' iniziale e 'size' a 0
void init_path(path_t *path);

// Riempie il percorso e rialloca la memoria se necessario
void add_move(path_t *path);

// Stampa il percorso
void print_path(path_t *path);

// Operazioni di conclusione partita AI
void finish_AI(char **maze, int x, path_t *path);

// Trova l'entrata e l'uscita del maze dato in input dall'utente
void find_entrance_exit(char **maze, int x, int y);

// Funzione 'move' ma utilizzata nell'AI sempre a destra
void move_right_hand(char **maze, int x, int y, path_t *path);

// Funzione di controllo celle adiacenti per la strategia AI Random
void check_near_cells_N(char **maze, int y, int *cell, neighbors_t cells);

// Funzione di controllo celle adiacenti per la strategia AI Random
void check_near_cells_S(char **maze, int x, int y, int *cell, neighbors_t cells);

// Funzione di controllo celle adiacenti per la strategia AI Random
void check_near_cells_E(char **maze, int x, int y, int *cell, neighbors_t cells);

// Funzione di controllo celle adiacenti per la strategia AI Random
void check_near_cells_O(char **maze, int y, int *cell, neighbors_t cells);

// funzione 'move' ma utilizzata nell'AI random
void move_random(char **maze, int x, int y, path_t *path);

// Traccia sulla matrice (cella per cella) il percorso recentemente eseguito dallo Snake sovrascrivendo il contenuto con '.'
void mark_path(path_t *path, char **maze);

#endif