#include "animations.h"
#include "sprites.h"

/**
 * Animazione di sconfitta ( Game Over )
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void game_over(WINDOW *w1,int x,int y){

    /* Inizializzazione variabili */
	const char string[] = "GAME OVER"; //Stringa di Game Over
	int i; // Contatore

    /* Animazione iniziale */
    wclear(w1); // Pulizia iniziale dello schermo
	for(i=0;i < GM_CYCLES;i++){
		printnavetta_distrutta_f1(x,y,w1); // Stampa frame navetta distrutta, frame 1
		wbkgd(w1,COLOR_PAIR(WHI_RED)); // Sfondo: BIANCO/ROSSO
		wrefresh(w1);
		napms(GM_SPEED-50); // Delay per l'animazione

		printnavetta_distrutta_f2(x,y,w1); // Stampa frame navetta distrutta, frame 2
		wbkgd(w1,COLOR_PAIR(RED_YEL)); // Sfondo: ROSSO/GIALLO
		wrefresh(w1);
		napms(GM_SPEED-50); // Delay per l'animazione
	}
	
	/* Inizio animazione morte lenta - Rosso chiaro */
	napms(GM_DEATH); // Delay per l'animazione
	printnavetta_distrutta_f3(x,y,w1); // Stampa frame navetta distrutta, frame 3
	wbkgd(w1,COLOR_PAIR(WHI_LRED)); // Sfondo: BIANCO/ROSSO CHIARO
	wrefresh(w1);

	/* Inizio animazione morte lenta - Rosso scuro */
    napms(GM_DEATH-GM_SPEED); // Delay per l'animazione
	printnavetta_distrutta_f4(x,y,w1); // Stampa frame navetta distrutta, frame 4
	wbkgd(w1,COLOR_PAIR(WHI_DRED)); // Sfondo: BIANCO/ROSSO SCURO
	wrefresh(w1);

    /* Inizio animazione lenta - Sfondo nero */
    napms(GM_DEATH-GM_SPEED); // Delay per l'animazione
    wbkgd(w1,COLOR_PAIR(WHITE_BLACK)); // Sfondo: BIANCO:NERO
    wclear(w1); // Pulizia schermo
	wrefresh(w1);
	napms(7*GM_SPEED); // Delay per l'animazione

	sleep(2); // Delay per l'animazione

	/* Stampa scritta GAME OVER. */
	for(i=0;i < GM_CYCLES_L-1;i++){
        /* Stampa carattere per carattere la stringa GAME OVER */
		mvwaddch(w1,LINES/2,(COLS/2+i)-5,string[i]);
		wrefresh(w1);
		napms(3*GM_SPEED); // Delay per l'animazione
        wclear(w1);
	}
	wclear(w1); 
	wrefresh(w1);
	napms(3*GM_SPEED); // Delay per l'animazione

	/* Stampa della stringa GAME OVER per intero */
	mvwprintw(w1,LINES/2,(COLS/2)-5,string);
	wrefresh(w1);
	sleep(3); // Delay per l'animazione
    
    /* Fine esecuzione */
	wclear(w1); 
	wrefresh(w1);
}

/**
 * Animazione di vittoria ( Victory )
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void victory(WINDOW *w1,int x,int y){
    
    /* Inizializzazione variabili */
    const char string[] = "VICTORY"; // Stringa di vittoria
	int i, j; // Contatori
    int maxx, maxy; // Variabili per la risoluzione schermo
    getmaxyx(w1,maxy,maxx); // FUnzione per la risoluzione schermo

    /* Animazione iniziale */
    wclear(w1); //Avvio tutto pulendo lo schermo
	for(i=0,j=2;j < maxx/2;i++,j+=1){
        printnavetta(x+j,y,w1); // Stampa frame della navetta
        wbkgd(w1,COLOR_PAIR(WHI_GR)); // Sfondo: BIANCO/VERDE
		wrefresh(w1);
		napms(GM_SPEED/20); // Delay per l'animazione

		wbkgd(w1,COLOR_PAIR(WHI_BL));  // Sfondo: BIANCO/BLU
		wrefresh(w1);
		napms(GM_SPEED/20); // Delay per l'animazione
        wclear(w1);
	}

	/* Inizio animazione lenta - Verde chiaro */
	napms(GM_DEATH); // Delay per l'animazione
	wbkgd(w1,COLOR_PAIR(WHI_LGR));  // Sfondo: BIANCO/VERDE CHIARO
	wrefresh(w1);
    
	/* Inizio animazione lenta - Verde scuro */
    napms(GM_DEATH-GM_SPEED); // Delay per l'animazione
	wbkgd(w1,COLOR_PAIR(WHI_DGR));  // Sfondo: BIANCO/VERDE SCURO
	wrefresh(w1);

    /* Inizio animazione lenta - Sfondo nero */
	napms(GM_DEATH-GM_SPEED); // Delay per l'animazione
	wbkgd(w1,COLOR_PAIR(WHITE_BLACK));  // Sfondo: BIANCO/NERO
    wrefresh(w1);
    
    
	sleep(2); // Delay per l'animazione
    wclear(w1);

	/* Stampa scritta VICTORY carattere per carattere */
	for(i=0;i < GM_CYCLES_W-1;i++){
		mvwaddch(w1,LINES/2,(COLS/2+i)-5,string[i]); // Stampa carattere
		wrefresh(w1);
		napms(2*GM_SPEED); // Delay per l'animazione
        wclear(w1);
	}
	napms(3*GM_SPEED); // Delay per l'animazione

	/* Stampa della stringa VICTORY per intero che va a dx e sx
	 * Insieme a 4 navicelle nemiche che la circondano */
    for(i=0;i<6;i++) {
        mvwprintw(w1,maxy/2,(maxx/2)-5,string);
        printnemicolv1_f1(maxx / 3, maxy / 3, w1); //Navicella sx alto
        printnemicolv1_f1(2*(maxx / 3), maxy / 3, w1); //Navicella dx alto
        printnemicolv1_f1(maxx / 3, 2*(maxy / 3), w1); //Navicella sx basso
        printnemicolv1_f1(2*(maxx / 3), 2*(maxy / 3), w1); //Navicella dx basso
        wrefresh(w1);
        napms(3*GM_SPEED); // Delay per l'animazione

        wclear(w1);
        mvwprintw(w1,LINES/2,(COLS/2),string);
        printnemicolv1_f2(maxx / 3, maxy / 3, w1); //Navicella sx alto
        printnemicolv1_f2(2*(maxx / 3), maxy / 3, w1); //Navicella dx alto
        printnemicolv1_f2(maxx / 3, 2*(maxy / 3), w1); //Navicella sx basso
        printnemicolv1_f2(2*(maxx / 3), 2*(maxy / 3), w1); //Navicella dx basso
        wrefresh(w1);
        napms(3*GM_SPEED); // Delay per l'animazione
        
        wclear(w1);
    }
}
