#include "animations.h"
#include "sprites.h"

// Questa sezione di codice serve per le animazioni generali

//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori

/* game_over = Animazione di gameover.
 * int x prende le ascisse della navetta attuali(potrebbe non servire).
 * int y prende le ordinate della navetta attuali.*/
void game_over(WINDOW *w1,int x,int y){
	//clear iniziale
	wclear(w1);

	const char string[] = "GAME OVER";
	char tmp[2] = {' ', '\0'};
	int i = 0;

	//Definizione di colori


	//Animazione schermo che flasha
	for(i=0;i < GM_CYCLES;i++){
		//Inizio frame sfondo giallo
		printnavetta_distrutta_f1(x,y,w1);
		wbkgd(w1,COLOR_PAIR(WHI_RED));
        refresh();
		wrefresh(w1);
		//Fine frame sfondo giallo
		napms(GM_SPEED-50);

		//Inizio frame sfondo rosso
		printnavetta_distrutta_f2(x,y,w1);
		wbkgd(w1,COLOR_PAIR(RED_YEL));
        refresh();
		wrefresh(w1);
		//Fine frame sfondo rosso
		napms(GM_SPEED-50);
	}
	
	//Inizio animazione morte lenta - Rosso chiaro
	napms(GM_DEATH);
	printnavetta_distrutta_f3(x,y,w1);
	wbkgd(w1,COLOR_PAIR(WHI_LRED));
	wrefresh(w1);
	napms(GM_DEATH-GM_SPEED);
     //Fine

	//Inizio animazione morte lenta - Rosso scuro
	printnavetta_distrutta_f4(x,y,w1);
	wbkgd(w1,COLOR_PAIR(WHI_DRED));
	wrefresh(w1);
	napms(GM_DEATH-GM_SPEED);
	wbkgd(w1,COLOR_PAIR(WHITE_BLACK));
    wclear(w1);
	wrefresh(w1);
	napms(7*GM_SPEED);
	//Fine

	wclear(w1); 
	wrefresh(w1); //Fine

	sleep(2);

	// Stampa scritta GAME OVER.
	for(i=0;i < GM_CYCLES_L-1;i++){
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

void victory(WINDOW *w1,int x,int y){
    const char string[] = "VICTORY";
	int i, j;
    int maxx, maxy;
    getmaxyx(w1,maxy,maxx);

    wclear(w1); //Avvio tutto pulendo lo schermo

	//Animazione schermo che flasha
	for(i=0,j=2;j < maxx/2;i++,j+=1){
		//Inizio frame sfondo Verde
        printnavetta(x+j,y,w1);
        wbkgd(w1,COLOR_PAIR(WHI_GR));
		wrefresh(w1);
		//Fine frame sfondo Verde
		napms(GM_SPEED/20);

		//Inizio frame sfondo Blu
		wbkgd(w1,COLOR_PAIR(WHI_BL));
		wrefresh(w1);
		//Fine frame sfondo Blu
		napms(GM_SPEED/20);

        wclear(w1); //Rinizia il ciclo e la navetta si sposta
	}

    x+=j-5; //Riprende le ultime coordinate
	//Inizio animazione lenta - Verde chiaro
	napms(GM_DEATH);
	wbkgd(w1,COLOR_PAIR(WHI_LGR));
	wrefresh(w1);
    //Fine

	//Inizio animazione lenta - Verde scuro
    napms(GM_DEATH-GM_SPEED);
	wbkgd(w1,COLOR_PAIR(WHI_DGR));
	wrefresh(w1);
    //Fine

    //Inizio animazione lenta - Sfondo nero
	napms(GM_DEATH-GM_SPEED);
    wclear(w1); 
	wbkgd(w1,COLOR_PAIR(WHITE_BLACK));
    wrefresh(w1);
	sleep(2);
    wclear(w1);
    //Fine

	// Stampa scritta VICTORY.
	for(i=0;i < GM_CYCLES_W-1;i++){
		mvwaddch(w1,LINES/2,(COLS/2+i)-5,string[i]);
		wrefresh(w1);
		napms(2*GM_SPEED);
        wclear(w1);
	}
	napms(3*GM_SPEED);

	/* Stampa della stringa VICTORY per intero che va a dx e sx
	 * Insieme a 4 navicelle nemiche che la circondano */
    for(i=0;i<6;i++) {
        mvwprintw(w1,maxy/2,(maxx/2)-5,string);
        printnemicolv1_f1(maxx / 3, maxy / 3, w1); //Navicella sx alto
        printnemicolv1_f1(2*(maxx / 3), maxy / 3, w1); //Navicella dx alto
        printnemicolv1_f1(maxx / 3, 2*(maxy / 3), w1); //Navicella sx basso
        printnemicolv1_f1(2*(maxx / 3), 2*(maxy / 3), w1); //Navicella dx basso
        wrefresh(w1);
        napms(3*GM_SPEED);

        wclear(w1);
        mvwprintw(w1,LINES/2,(COLS/2),string);
        printnemicolv1_f2(maxx / 3, maxy / 3, w1); //Navicella sx alto
        printnemicolv1_f2(2*(maxx / 3), maxy / 3, w1); //Navicella dx alto
        printnemicolv1_f2(maxx / 3, 2*(maxy / 3), w1); //Navicella sx basso
        printnemicolv1_f2(2*(maxx / 3), 2*(maxy / 3), w1); //Navicella dx basso
        wrefresh(w1);
        napms(3*GM_SPEED);
        
        wclear(w1);
    }
    //Fine dell'esecuzione
}
