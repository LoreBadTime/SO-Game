#ifndef PROC_H
#define PROC_H
#include "../globalincludes.h"

/* Definizione MACRO */
#define ENEM_TEST 30 // Massimo numero di nemici giocabili
#define MAX_PROIETTILI 2 // Massimo numero di proiettili sparati ( non modificabile )

/* Funzioni versione processi */
void proiettile(WINDOW* w,int x, int y, int direzione, int *pipe);
void bomba(WINDOW* w,int x, int y, int id,int *pipe);
void nave(int *pipe, int sys_slownes);
void nemico(int x,int y,int id,int direzione,int *sender,int *receiver);
void screen(WINDOW *w1, int num_nemici, int rimbalzi, int colore);

#endif
