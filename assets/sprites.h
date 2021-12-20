#ifndef SPRITES_H
#define SPRITES_H
#include "../globalincludes.h"

void printnavetta(int x,int y,WINDOW *w);
void printnavetta_distrutta_f1(int x,int y,WINDOW *w);
void printnavetta_distrutta_f2(int x,int y,WINDOW *w);
void printnavetta_distrutta_f3(int x,int y,WINDOW *w);
void printnavetta_distrutta_f4(int x,int y,WINDOW *w);
void printnemicolv2_f1(int x,int y,WINDOW *w);
void printnemicolv2_f2(int x,int y,WINDOW *w);
void printnemicolv1_f1(int x,int y,WINDOW *w);
void printnemicolv1_f2(int x,int y,WINDOW *w);
void print_info(int proiettile_pronto, int life,WINDOW *w1,int maxx);
int print_nave(int invincibility,WINDOW *w1,int x,int y);
void main_test();
#endif
