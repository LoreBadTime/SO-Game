#ifndef SO_GAME_MAIN_THREADS_H
#define SO_GAME_MAIN_THREADS_H

#include "../globalincludes.h"

/* Dichiarazioni delle funzioni threads */
void* thread_proiettile(void* p_proiettile);
void* thread_bomba(void* p_bomba);
void* thread_nave(void *parametro);
void* thread_nemico(void* p_nemico);
void screen_threads(WINDOW *w1, int num_nemici, int vite, int colore);

#endif //SO_GAME_MAIN_THREADS_H
