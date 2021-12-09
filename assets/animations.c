#include "animations.h"
//velocita e impostazione animazione

//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori

/* game_over = Animazione di gameover.
 * int x prende le ascisse della navetta attuali(potrebbe non servire).
 * int y prende le ordinate della navetta attuali.*/
void game_over(WINDOW *w1,int x,int y){
	wclear(w1);
	wrefresh(w1);
	//print sprite with x and y
	
	const char string[] = "GAME OVER";
	char tmp[2] = {' ', '\0'};
	int i = 0;

	//Definizione colori
	init_color(COLOR_BLACK,0, 0, 0);
	init_color(GM_LESS_DARKER_RED,125, 0, 0);
	init_color(GM_MORE_DARKER_RED,46, 9, 8);
	//Definizione coppie di colori
	init_pair(1,COLOR_RED,COLOR_YELLOW);
	init_pair(2,COLOR_WHITE,COLOR_RED);
	init_pair(3,COLOR_WHITE,GM_LESS_DARKER_RED);
	init_pair(4,COLOR_WHITE,GM_MORE_DARKER_RED);
	init_pair(5,COLOR_WHITE,COLOR_BLACK);

	//Animazione schermo che flasha
	for(i=0;i < GM_CYCLES;i++){
		//Inizio frame sfondo giallo
		mvwprintw(w1,y,x,NAVE_OK);
		wbkgd(w1,COLOR_PAIR(1));
        refresh();
		wrefresh(w1);
		//Fine frame sfondo giallo
		napms(GM_SPEED);

		//Inizio frame sfondo rosso
		mvwprintw(w1,y,x,NAVE_ESPLOSA);
		wbkgd(w1,COLOR_PAIR(2));
        refresh();
		wrefresh(w1);
		//Fine frame sfondo rosso
		napms(GM_SPEED);
	}
	
	//Inizio animazione morte lenta - Rosso chiaro
	napms(GM_DEATH);
	mvwprintw(w1,y,x,NAVE_ESPLOSA);
	wbkgd(w1,COLOR_PAIR(3));
    refresh();
	wrefresh(w1);
	napms(GM_DEATH-GM_SPEED);
     //Fine

	//Inizio animazione morte lenta - Rosso scuro
	mvwprintw(w1,y,x,NAVE_ESPLOSA);
	wbkgd(w1,COLOR_PAIR(4));
    refresh();
	wrefresh(w1);
	napms(GM_DEATH-GM_SPEED);
    //Fine

	//Inizio animazione morte lenta - Nero
	mvwprintw(w1,y,x,NAVE_ESPLOSA);
	wbkgd(w1,COLOR_PAIR(5));
    refresh();
	wrefresh(w1);
	napms(7*GM_SPEED);
	wclear(w1); 
    refresh();
	wrefresh(w1); //Fine

	sleep(3);	

	/* Stampa lettera per lettera della stringa GAME OVER.
	 * mvprintw accepts only strings, so i need to convert 
	 * single char into strings to make the animation */
	for(i=0;i < GM_CYCLES2-1;i++){
		mvwaddch(w1,LINES/2,(COLS/2+i)-5,string[i]);
		wrefresh(w1);
		napms(3*GM_SPEED);
        wclear(w1);
	}
	wclear(w1); 
	wrefresh(w1);
	napms(3*GM_SPEED);

	//Stampa della stringa GAME OVER per intero
	mvwprintw(w1,LINES/2,(COLS/2)-5,string);
	wrefresh(w1);
	sleep(3);
    
    //Fine dell'esecuzione
	wclear(w1); 
	wrefresh(w1);
}
