#ifndef SO_GAME_MAIN_THREADS_H
#define SO_GAME_MAIN_THREADS_H

#include "../globalincludes.h"

#define ENEM_TEST 30 //con + di 8 nemici lo spawner attuale impazzisce,necessario algoritmo migliore
#define MAX_PROIETTILI 2
int enemyLV1(int x,int y,int id,int direzione,int *sender,int *receiver);
void *getinput();
void screen_threads(WINDOW *w1, int num_nemici, int rimbalzi, int colore);


#endif //SO_GAME_MAIN_THREADS_H
