#ifndef SPRITES_H
#define SPRITES_H
#include "../globalincludes.h"

/* Stampe frame nave principale */
void printnavetta(int x,int y,WINDOW *w);
void printnavetta_distrutta_f1(int x,int y,WINDOW *w);
void printnavetta_distrutta_f2(int x,int y,WINDOW *w);
void printnavetta_distrutta_f3(int x,int y,WINDOW *w);
void printnavetta_distrutta_f4(int x,int y,WINDOW *w);

/* Stampe frame navi nemiche */
void printnemicolv2_f1(int x,int y,WINDOW *w);
void printnemicolv2_f2(int x,int y,WINDOW *w);
void printnemicolv2_f3(int x,int y,WINDOW *w);
void printnemicolv2_f4(int x,int y,WINDOW *w);
void printnemicolv1_f1(int x,int y,WINDOW *w);
void printnemicolv1_f2(int x,int y,WINDOW *w);

/* Stampe varie a schermo */
void print_info(int proiettile_pronto, int life,WINDOW *w1,int maxx,int maxy);
int print_nave(int invincibility,WINDOW *w1,int x,int y);
void print_titolo(WINDOW* w,int x, int y);

#endif
