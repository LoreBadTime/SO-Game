#ifndef GLOBALINCLUDES
#define GLOBALINCLUDES
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include "./assets/animations.h"
#include "./assets/sprites.h"
#include "./versione_processi/process.h"
#include "./versione_threads/threads.h"

#define ENEM_TEST 30 // Massimo numero di nemici giocabili
#define MAX_PROIETTILI 2 // Massimo numero di proiettili sparati ( non modificabile )
#define pos_X 0
#define pos_Y 1
#define SU 65 /* Freccia su */
#define GIU 66 /* Freccia giu */
#define DIM 5
#define PRONTO 1 // Segnala se il proiettile è pronto ad essere sparato
#define LARGHEZZA 3 // Larghezza dello sprite dal suo centro (nel player grande 5 -> 2+(centro)=3)
#define DIAGONALE 8 // Ogni quante x l'ordinata del proiettile viene incrementata
#define VIVA 1 // Stato navicella nemica
#define UCCISA 0
#define BORDO 2 //La bomba ha raggiunto il bordo
#define SCARICO 0 //Il caricatore dei proiettili principali o delle bombe è scarico
#define PROIETTILE_BASSO 0 //ID del proiettile che avanza verso il basso
#define PROIETTILE_ALTO 1 //ID del proiettile che avanza verso l'alto
#define MAX_VITE 10 //Vite massimi selezionabili

#define GM_LESS_DARKER_RED 9
#define GM_MORE_DARKER_RED 10
#define GM_LESS_DARKER_GREEN 11
#define GM_MORE_DARKER_GREEN 12
#define CYAN 13

#define WHITE_BLACK 1  
#define RED_YEL 2
#define WHI_RED 3
#define WHI_LRED 4
#define WHI_DRED 5
#define WHI_BL 6
#define WHI_GR 7
#define WHI_LGR 8
#define WHI_DGR 9 
#define CY_BL 10
#define YEL_BL 11
#define GRE_BL 12
#define RED_BL 13
#define CY_GRE 14
#define CY_BLU 15
#define DARKRED_BL 16

typedef struct {
  int x;        // Coordinata x
  int y;        // Coordinata y
} Posizione;

typedef struct {
	int x;
	int y;
	char* c[DIM];
} oggetto;

typedef struct {
	int x;
	int y;
	int riconoscimento;
    int ready;
    int id;
} Bullet;

typedef struct {
  Posizione navnemica;
  Bullet proiettile;
} Navetta_Nemica;

typedef struct {
	Posizione coordinata;
	Bullet proiettile;
	int angolo;
	int id;
} Player;


void stampanemici(WINDOW* w1,Player arr,int frame);
void menu();

#endif
