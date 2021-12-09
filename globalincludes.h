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
#include "./assets/animations.h"
#include "./assets/sprites.h"
#include "./versione_processi/process.h"

#define pos_X 0
#define pos_Y 1
#define SU 65 /* Freccia su */
#define GIU 66 /* Freccia giu */
#define DIM 5

typedef struct {
  int x;        // Coordinata x
  int y;        // Coordinata y
} Posizione;

typedef struct {
	int x;
	int y;
	char* c[DIM-1];
} oggetto;

typedef struct {
	int x;
	int y;
	char* c[DIM-1];
	int ready;
} Bullet;

void menu();

#endif
