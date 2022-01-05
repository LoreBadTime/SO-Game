#ifndef PROC_H
#define PROC_H

#include "../globalincludes.h"

/* Funzioni versione processi */
void proiettile(WINDOW* w,int x, int y, int direzione, int *pipe);
void bomba(WINDOW* w,int x, int y, int id,int *pipe);
void nave(int *pipe, int sys_slownes);
void nemico(int x,int y,int id,int direzione,int *sender,int *receiver);
void screen(WINDOW *w1, int num_nemici, int vite, int colore);

#endif
