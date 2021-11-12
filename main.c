#include "./globalincludes.h"
#include "./assets/animations.h"

typedef struct {
  int x;        // Coordinata x
  int y;        // coordinata y
} Posizione;

void menu();

void main(){
    menu();
}

void menu(){
	/* Variabili */
	int maxx=0;
	int maxy=0;
	int key=0;
	WINDOW *w1;

	/* Inizializzazione schermo */
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, 1);
	
	/* Personalizzazione colori e dimensioni dello schermo */
	getmaxyx(stdscr, maxy, maxx);
	w1 = newwin (maxy,maxx,0,0);
	start_color();
	init_pair(1,COLOR_YELLOW,COLOR_BLACK);
	wbkgd(w1,COLOR_PAIR(1));

	/* Creazione indicatore e coordinate per il menù */
	Posizione oggetto = { (maxx/2)-12,(maxy/2)+1 };
	mvwprintw(w1,oggetto.y,oggetto.x,"-");
	int ymod=(maxy/2)-5; //Ordinate scritta indicativa
	int yext=(maxy/2)+1; //Ordinate scritta EXIT
	int ygov=(maxy/2)+2; //Ordinate scritta Game-Over
	int yflscr=(maxy/2)+3; //Ordinate scritta Schermo COmpleto
	int xbase=(maxx/2)-10; //Ascisse di default



	/* Selezione menù */
	while (true) {
			/* Creazioni grafiche per il menù */
			box(w1,ACS_VLINE,ACS_HLINE);
			mvwprintw(w1,ymod,xbase,"Inserire una modalità:");
			mvwprintw(w1,yext,xbase,"EXIT");
			mvwprintw(w1,ygov,xbase,"Game-Over");
			mvwprintw(w1,yflscr,xbase,"Schermo completo");
			wrefresh(w1);

			/* Opzione tasti di movimento e selezione */
			key = wgetch(w1);
		
			switch(key) {
			      	case 65: //Freccia verso l'alto
					if(oggetto.y == yext){oggetto.y=yflscr;} //Ritorno dalla prima all'ultima opzione
					else{ if(oggetto.y > 0) oggetto.y -= 1; }
					break;

				case 66: //Freccia verso il basso
					if(oggetto.y == yflscr){oggetto.y=yext;} //Ritorno dall'ultima alla prima opzione
					else{ if(oggetto.y < maxy - 1) oggetto.y += 1; }
					break;

				case 10: //Tasto invio
					if(oggetto.y == yext){ //Scelta menù = EXIT
				     		endwin();
						exit(0);
					}
					if(oggetto.y == ygov){ //Scelta menù = Animazione Game Over
						wclear(w1); 
						game_over(w1,1,11);
						wclear(w1);
						endwin();
						exit(0);
					}
					if(oggetto.y == yflscr){ //Scelta menù = Animazione completa
						screen(w1);
					}
					break; 
				}

		/* Spostamento del cursore in base al tasto premuto */
		wclear(w1);
		mvwaddch(w1,oggetto.y, oggetto.x, '-');
		}
}
