#include "animations.h"
//velocita e impostazione animazione

//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori


/* game_over = Animazione di gameover.
 * int x prende le ascisse della navetta attuali(potrebbe non servire).
 * int y prende le ordinate della navetta attuali.*/
void game_over(int x,int y){
	clear();
	refresh();
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
		mvprintw(y,x,NAVE_OK);
		bkgd(COLOR_PAIR(1));
		refresh();
		//Fine frame sfondo giallo
		napms(GM_SPEED);
		clear();
		refresh();

		//Inizio frame sfondo rosso
		mvprintw(y,x,NAVE_ESPLOSA);
		bkgd(COLOR_PAIR(2));
		refresh();
		//Fine frame sfondo rosso
		napms(GM_SPEED);
		clear();
	}
	
	//Inizio animazione morte lenta - Rosso chiaro
	napms(GM_DEATH);
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(3));
	refresh();
	napms(GM_DEATH-GM_SPEED);
	clear();
	refresh(); //Fine

	//Inizio animazione morte lenta - Rosso scuro
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(4));
	refresh();
	napms(GM_DEATH-GM_SPEED);
	clear();
	refresh(); //Fine

	//Inizio animazione morte lenta - Nero
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(5));
	refresh();
	napms(7*GM_SPEED);
	clear(); 
	refresh(); //Fine

	sleep(3);	

	/* Stampa lettera per lettera della stringa GAME OVER.
	 * mvprintw accepts only strings, so i need to convert 
	 * single char into strings to make the animation */
	for(i=0;i < GM_CYCLES2;i++){
		tmp[1] = string[i];
		mvprintw(LINES/2,(COLS/2+i)-5,tmp);
		refresh();
		napms(3*GM_SPEED);
	}
	clear(); 
	refresh();
	napms(3*GM_SPEED);

	//Stampa della stringa GAME OVER per intero
	mvprintw(LINES/2,(COLS/2)-5,string);
	refresh();
	sleep(3);
    
    //Fine dell'esecuzione
	clear(); 
	refresh();
}
