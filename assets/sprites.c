#include "sprites.h"
//stampa navetta in generale
void printnavetta(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|\\");
	mvwprintw(w,y-1,x-5,"| \\");
	mvwprintw(w,y,x-5,"|-O|==");
	mvwprintw(w,y+1,x-5,"| /");
	mvwprintw(w,y+2,x-5,"|/");
}
//animazioni navetta gameover (frame 1)
void printnavetta_distrutta_f1(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|X");
	mvwprintw(w,y-1,x-5,"|X\\");
	mvwprintw(w,y,x-5,"X-O|X=");
	mvwprintw(w,y+1,x-5,"| X");
	mvwprintw(w,y+2,x-5,"X/");
}
//animazioni navetta gameover (frame 2)
void printnavetta_distrutta_f2(int x,int y,WINDOW *w){	
	mvwprintw(w,y-2,x-5,"|\\ ");
	mvwprintw(w,y-1,x-5,"X X ");
	mvwprintw(w,y,x-5,  "|-X|=X");
	mvwprintw(w,y+1,x-5,"|X/");
	mvwprintw(w,y+2,x-5,"X/");
}
//animazioni navetta gameover (frame 3)
void printnavetta_distrutta_f3(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|\\ ");
	mvwprintw(w,y-1,x-5,"     ");
	mvwprintw(w,y,x-5,  "|- |=   ");
	mvwprintw(w,y+1,x-5,"| /");
	mvwprintw(w,y+2,x-5," /");
}
//animazioni navetta gameover (frame 4)
void printnavetta_distrutta_f4(int x,int y,WINDOW *w){
	mvwprintw(w,y-2,x-5,"|   ");
	mvwprintw(w,y-1,x-5,"    ");
	mvwprintw(w,y,x-5,  " - |  ");
	mvwprintw(w,y+1,x-5,"|   ");
	mvwprintw(w,y+2,x-5,"   ");
}
//animazione nemico lv 2 di template per collisioni frame 1
void printnemicolv2_f1(int x,int y,WINDOW *w){
	mvwprintw(w,y-1,x,"O-O");
	mvwprintw(w,y,x,  "/V\\");
	mvwprintw(w,y+1,x,"O-O");
}
//animazione nemico lv 2 di template per collisioni frame 2
void printnemicolv2_f2(int x,int y,WINDOW *w){
	mvwprintw(w,y-1,x,"O-O");
	mvwprintw(w,y,x,  "|X|");
	mvwprintw(w,y+1,x,"O-O");
}

//animazione nemico lv 2 di template per collisioni frame 3
void printnemicolv2_f3(int x,int y,WINDOW *w){
    mvwprintw(w,y-1,x,"O X");
    mvwprintw(w,y,x,  " X ");
    mvwprintw(w,y+1,x,"X O");
}

//animazione nemico lv 2 di template per collisioni frame 4
void printnemicolv2_f4(int x,int y,WINDOW *w){
    mvwprintw(w,y-1,x,"X O");
    mvwprintw(w,y,x,  " X ");
    mvwprintw(w,y+1,x,"O X");
}


//animazione nemico lv 1 di template per collisioni frame 1
void printnemicolv1_f1(int x,int y,WINDOW *w){
	mvwprintw(w,y-1,x, " /O");
	mvwprintw(w,y,x,   "-O ");
	mvwprintw(w,y+1,x," \\O");
}
//animazione nemico lv 1 di template per collisioni frame 2
void printnemicolv1_f2(int x,int y,WINDOW *w){
	mvwprintw(w,y-1,x," \\O");
	mvwprintw(w,y,x,  "-O ");
	mvwprintw(w,y+1,x," /O");
}

/**
 * Stampa dei nemici in base alla quantità delle loro vite
 *
 * WINDOW* w1 : Finestra di stampa.
 * Player arr : Array di nemici. */
void stampanemici(WINDOW* w1, Player arr) {
    /* Se il nemico è ancora vivo, avrà un frame diverso per ogni ordinata
     * Per fare ciò si usa l'operatore di modulo */
    if (arr.coordinata.y % 2 == 0 && arr.coordinata.y) {
        if (arr.proiettile.id == 3) { //Se al nemico rimangono tre vite
            wattron(w1, COLOR_PAIR(GRE_BL));
            printnemicolv1_f1(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(GRE_BL));
        }
        if (arr.proiettile.id == 2) { //Se al nemico rimangono due vite
            wattron(w1, COLOR_PAIR(YEL_BL));
            printnemicolv2_f1(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(YEL_BL));
        }
        if (arr.proiettile.id == 1) { //Se al nemico rimane una vita
            wattron(w1, COLOR_PAIR(RED_BL));
            printnemicolv2_f3(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(RED_BL));
        }
    } else {
        if (arr.proiettile.id == 3) { //Se al nemico rimangono tre vite
            wattron(w1, COLOR_PAIR(GRE_BL));
            printnemicolv1_f2(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(GRE_BL));
        }
        if (arr.proiettile.id == 2) { //Se al nemico rimangono due vite
            wattron(w1, COLOR_PAIR(YEL_BL));
            printnemicolv2_f2(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(YEL_BL));
        }
        if (arr.proiettile.id == 1) { //Se al nemico rimane una vita
            wattron(w1, COLOR_PAIR(RED_BL));
            printnemicolv2_f4(arr.coordinata.x, arr.coordinata.y, w1);
            wattroff(w1, COLOR_PAIR(RED_BL));
        }
    }

    wattron(w1, COLOR_PAIR(RED_BL));
    mvwaddch(w1, arr.proiettile.y, arr.proiettile.x, 'O');
    wattroff(w1, COLOR_PAIR(RED_BL));
}


//Stampa numero di vite e disponibilita proiettili a schermo
void print_info(int proiettile_pronto,int life,WINDOW *w1,int maxx, int maxy){
    int i;
    mvwprintw(w1, 0, 1, "Proiettile:" );
    if(proiettile_pronto != PRONTO) {
        mvwaddch(w1, 0, 13, ' ');
    }
    else {
        wattron(w1, COLOR_PAIR(GRE_BL));
        mvwaddch(w1, 0, 13, 'O');
        wattroff(w1, COLOR_PAIR(GRE_BL));
    }

    mvwprintw(w1, 0 , 16, "Vite:");
	for (i=0;i < life;i++){
        wattron(w1, COLOR_PAIR(RED_BL));
		mvwaddch(w1, 0, 22 + i, '#');
        wattroff(w1, COLOR_PAIR(RED_BL));
	}
    for(i=0;i<maxx + 3;i++){
        mvwaddch(w1,1,i,'_');
    }

    for(i=2;i<maxy;i++){
        wattron(w1, COLOR_PAIR(DARKRED_BL));
        mvwaddch(w1,i,11,'|');
        wattroff(w1, COLOR_PAIR(DARKRED_BL));
    }
}

//algoritmo di stampa navetta con invincibilita
int print_nave(int invincibility,WINDOW *w1,int x,int y){
	if(invincibility){
		--invincibility;
		if (invincibility % 10 < 7){
			printnavetta(x, y, w1);
		}
	}
	else{
		printnavetta(x, y, w1);
	}
	return invincibility;
}

void print_titolo(WINDOW* w,int x, int y) {
    mvwprintw(w, y, x, " ____  ____   _    ____ _____");
    mvwprintw(w, y + 1, x, "/ ___||  _ \\ / \\  / ___| ____|");
    mvwprintw(w, y + 2, x, "\\___ \\| |_) / _ \\| |   |  _|");
    mvwprintw(w, y + 3, x, " ___) |  __/ ___ \\ |___| |___");
    mvwprintw(w, y + 4, x, "|____/|_| /_/   \\_\\____|_____|");
    y+=5;
    mvwprintw(w, y, x, " ____  _____ _____ _____ _   _ ____  _____ ____ ");
    mvwprintw(w, y + 1, x, "|  _ \\| ____|  ___| ____| \\ | |  _ \\| ____|  _ \\ ");
    mvwprintw(w, y + 2, x, "| | | |  _| | |_  |  _| |  \\| | | | |  _| | |_) |");
    mvwprintw(w, y + 3, x, "| |_| | |___|  _| | |___| |\\  | |_| | |___|  _ < ");
    mvwprintw(w, y + 4, x, "|____/|_____|_|   |_____|_| \\_|____/|_____|_| \\_\\");

}
