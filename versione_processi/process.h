#ifndef PROC_H
#define PROC_H
#include "../globalincludes.h"
#define ENEM_TEST 5
//con + di 8 nemici lo spawner attuale impazzisce,necessario algoritmo migliore
int enemyLV1(int x,int y,int id,int direzione,int *sender,int *receiver);
void *getinput();
void screen(WINDOW *w1);
#endif