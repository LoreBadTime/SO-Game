#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "../globalincludes.h"


#define GM_SPEED 150
#define GM_CYCLES 5
#define GM_CYCLES_L 10
#define GM_CYCLES_W 8
#define GM_DEATH 1000

//Sprite per la navetta (da creare)
#define NAVE_OK "O"
#define NAVE_ESPLOSA "X"

void game_over(WINDOW *w1,int x,int y);
void victory(WINDOW *w1,int x,int y);

#endif
