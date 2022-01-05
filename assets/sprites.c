#include "sprites.h"

/**
 * Stampa titolo del gioco
 *
 * WINDOW* w1 : Finestra di stampa.
 * int x : Ascisse di generazione.
 * int y : Ordinate di generazione. */
void print_titolo(WINDOW* w,int x, int y) {
    mvwprintw(w, y, x, " ____  ____   _    ____ _____");
    mvwprintw(w, y + 1, x, "/ ___||  _ \\ / \\  / ___| ____|");
    mvwprintw(w, y + 2, x, "\\___ \\| |_) / _ \\| |   |  _|");
    mvwprintw(w, y + 3, x, " ___) |  __/ ___ \\ |___| |___");
    mvwprintw(w, y + 4, x, "|____/|_| /_/   \\_\\____|_____|");
    y+=5; // Andata a capo
    mvwprintw(w, y, x, " ____  _____ _____ _____ _   _ ____  _____ ____ ");
    mvwprintw(w, y + 1, x, "|  _ \\| ____|  ___| ____| \\ | |  _ \\| ____|  _ \\ ");
    mvwprintw(w, y + 2, x, "| | | |  _| | |_  |  _| |  \\| | | | |  _| | |_) |");
    mvwprintw(w, y + 3, x, "| |_| | |___|  _| | |___| |\\  | |_| | |___|  _ < ");
    mvwprintw(w, y + 4, x, "|____/|_____|_|   |_____|_| \\_|____/|_____|_| \\_\\");
}

/**
 * Funzione di stampa nave principale
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void printnavetta(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|\\");
	mvwprintw(w,y-1,x-5,"| \\");
	mvwprintw(w,y,x-5,"|-O|==");
	mvwprintw(w,y+1,x-5,"| /");
	mvwprintw(w,y+2,x-5,"|/");
}

/**
 * Funzione di stampa nave principale distrutta, frame 1
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void printnavetta_distrutta_f1(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|X");
	mvwprintw(w,y-1,x-5,"|X\\");
	mvwprintw(w,y,x-5,"X-O|X=");
	mvwprintw(w,y+1,x-5,"| X");
	mvwprintw(w,y+2,x-5,"X/");
}

/**
 * Funzione di stampa nave principale distrutta, frame 2
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void printnavetta_distrutta_f2(int x,int y,WINDOW *w){	
	mvwprintw(w,y-2,x-5,"|\\ ");
	mvwprintw(w,y-1,x-5,"X X ");
	mvwprintw(w,y,x-5,  "|-X|=X");
	mvwprintw(w,y+1,x-5,"|X/");
	mvwprintw(w,y+2,x-5,"X/");
}

/**
 * Funzione di stampa nave principale distrutta, frame 3
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void printnavetta_distrutta_f3(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|\\ ");
	mvwprintw(w,y-1,x-5,"     ");
	mvwprintw(w,y,x-5,  "|- |=   ");
	mvwprintw(w,y+1,x-5,"| /");
	mvwprintw(w,y+2,x-5," /");
}

/**
 * Funzione di stampa nave principale distrutta, frame 4
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 */
void printnavetta_distrutta_f4(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|   ");
	mvwprintw(w,y-1,x-5,"    ");
	mvwprintw(w,y,x-5,  " - |  ");
	mvwprintw(w,y+1,x-5,"|   ");
	mvwprintw(w,y+2,x-5,"   ");
}

/**
 * Animazione nemico livello 1, frame 1
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave nemica.
 * int y = Ordinata della nave nemica.
 */
void printnemicolv1_f1(int x,int y,WINDOW *w){
    mvwprintw(w,y-1,x, " /O");
    mvwprintw(w,y,x,   "-O ");
    mvwprintw(w,y+1,x," \\O");
}

/**
 * Animazione nemico livello 1, frame 1
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave nemica.
 * int y = Ordinata della nave nemica.
 */
void printnemicolv1_f2(int x,int y,WINDOW *w){
    mvwprintw(w,y-1,x," \\O");
    mvwprintw(w,y,x,  "-O ");
    mvwprintw(w,y+1,x," /O");
}

/**
 * Animazione nemico livello 2, frame 1
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave nemica.
 * int y = Ordinata della nave nemica.
 */
void printnemicolv2_f1(int x,int y,WINDOW *w){
	mvwprintw(w,y-1,x,"O^O");
	mvwprintw(w,y,x,  "/V\\");
	mvwprintw(w,y+1,x,"O^O");
}

/**
 * Animazione nemico livello 2, frame 2
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave nemica.
 * int y = Ordinata della nave nemica.
 */
void printnemicolv2_f2(int x,int y,WINDOW *w){
	mvwprintw(w,y-1,x,"O-O");
	mvwprintw(w,y,x,  "|X|");
	mvwprintw(w,y+1,x,"O-O");
}

/**
 * Animazione nemico livello 2, seconda vita, frame 1
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave nemica.
 * int y = Ordinata della nave nemica.
 */
void printnemicolv2_f3(int x,int y,WINDOW *w){
    mvwprintw(w,y-1,x,"O-O");
    mvwprintw(w,y,x,  " X ");
    mvwprintw(w,y+1,x,"O-O");
}

/**
 * Animazione nemico livello 2, seconda vita, frame 2
 * 
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della nave nemica.
 * int y = Ordinata della nave nemica.
 */
void printnemicolv2_f4(int x,int y,WINDOW *w){
    mvwprintw(w,y-1,x,"O O");
    mvwprintw(w,y,x,  "|X|");
    mvwprintw(w,y+1,x,"O O");
}

/**
 * Stampa dei nemici in base alla quantità delle loro vite
 *
 * WINDOW* w1 : Finestra di stampa.
 * Player arr : Array di nemici.
 * int frame : Frame attuale. */
void stampanemici(WINDOW* w1, Player arr, int frame) {
    /* Se il nemico è ancora vivo, avrà un frame diverso per ogni ordinata
     * Per fare ciò si usa l'operatore di modulo */
    if (frame % 2 == 0 && arr.coordinata.y) {
        if (arr.proiettile.id == 3) { //Se al nemico rimangono tre vite
            wattron(w1, COLOR_PAIR(GRE_BL)); // Start: VERDE/NERO
            printnemicolv1_f1(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(GRE_BL)); // End: VERDE/NERO
        }
        if (arr.proiettile.id == 2) { //Se al nemico rimangono due vite
            wattron(w1, COLOR_PAIR(YEL_BL)); // Start: GIALLO/NERO
            printnemicolv2_f1(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(YEL_BL)); // End: GIALLO/NERO
        }
        if (arr.proiettile.id == 1) { //Se al nemico rimane una vita
            wattron(w1, COLOR_PAIR(RED_BL)); // Start: ROSSO/NERO
            printnemicolv2_f3(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(RED_BL)); // End: ROSSO/NERO
        }
    } else {
        if (arr.proiettile.id == 3) { //Se al nemico rimangono tre vite
            wattron(w1, COLOR_PAIR(GRE_BL)); // Start: VERDE/NERO
            printnemicolv1_f2(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(GRE_BL)); // End: VERDE/NERO
        }
        if (arr.proiettile.id == 2) { //Se al nemico rimangono due vite
            wattron(w1, COLOR_PAIR(YEL_BL)); // Start: GIALLO/NERO
            printnemicolv2_f2(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(YEL_BL)); // End: GIALLO/NERO
        }
        if (arr.proiettile.id == 1) { //Se al nemico rimane una vita
            wattron(w1, COLOR_PAIR(RED_BL)); // Start: ROSSO/NERO
            printnemicolv2_f4(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(RED_BL)); // End: ROSSO/NERO
        }
    }

}

/**
 * Stampa delle informazioni principali a schermo
 * 
 * int proiettile_pronto : Indica se il proiettile della nave è pronto.
 * int life : Vite rimaste alla nave.
 * WINDOW *w1 : FInestra di stampa.
 * int maxx : Risoluzione in ascisse della finestra.
 * int maxy : Risoluzione in ascisse della finestra. */
void print_info(int proiettile_pronto,int life,WINDOW *w1,int maxx, int maxy){
    int i; // Contatore
    
    /* Stampa disponibilità proiettile */
    mvwprintw(w1, 0, 1, "Proiettile:" );
    if(proiettile_pronto != PRONTO) { // Se non è disponibile, non stampa
        mvwaddch(w1, 0, 13, ' ');
    }
    else { // Altrimenti stampa lo stato di disponibilità
        wattron(w1, COLOR_PAIR(GRE_BL)); // Start: VERDE/NERO
        mvwaddch(w1, 0, 13, 'O');
        wattroff(w1, COLOR_PAIR(GRE_BL)); // End: VERDE/NERO
    }

    /* Stampa numero di vite */
    mvwprintw(w1, 0 , 16, "Vite:");
	for (i=0;i < life;i++){
        wattron(w1, COLOR_PAIR(RED_BL)); // Start: ROSSO/NERO
		mvwaddch(w1, 0, 22 + i, '#');
        wattroff(w1, COLOR_PAIR(RED_BL)); // End: ROSSO/NERO
	}
    
    /* Stampa linea orizzontale */
    for(i=0;i<maxx + 3;i++){
        mvwaddch(w1,1,i,'_');
    }

    /* Stampa danger zone */
    for(i=2;i<maxy;i++){
        wattron(w1, COLOR_PAIR(DARKRED_BL)); // Start: ROSSO SCURO/NERO
        mvwaddch(w1,i,11,'|');
        wattroff(w1, COLOR_PAIR(DARKRED_BL)); // End: ROSSO SCURO/NERO
    }
}

/**
 * Funzione di invincibilità nave
 * 
 * int invincibiliy : Valore di invincibilità.
 * WINDOW *w1 : Finestra di stampa.
 * int x = Ascissa della nave.
 * int y = Ordinata della nave.
 * @return : Il valore di invincibilità.
 */
int print_nave(int invincibility,WINDOW *w1,int x,int y){
    
    /* La nave viene stampata a intervalli regolari per simulare la ripresa dal colpo */
	if(invincibility){
		--invincibility; // Decrementa il valore
		if (invincibility % 10 < 7){ // Usiamo il modulo per stampare la nave a intervalli
			printnavetta(x, y, w1);
		}
	}
	else{ // Se il valore è negativo stampa la navetta
		printnavetta(x, y, w1);
	}
	return invincibility; // Esegue il return del valore di invincibilità
}
