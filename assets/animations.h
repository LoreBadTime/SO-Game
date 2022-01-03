#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "../globalincludes.h"

/* MACRO di delay per l'animazione */
#define GM_SPEED 150
#define GM_CYCLES 5
#define GM_CYCLES_L 10
#define GM_CYCLES_W 8
#define GM_DEATH 1000

void game_over(WINDOW *w1,int x,int y); // Funzione di Game_Over
void victory(WINDOW *w1,int x,int y); // Funzione di vittoria

#endif
