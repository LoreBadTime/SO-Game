#include "./globalincludes.h"

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
	
	//
	getmaxyx(stdscr, maxy, maxx);
	w1 = newwin (maxy,maxx,0,0);
	start_color();
	// definizione colori
	init_color(COLOR_BLACK, 0, 0, 0);
	init_color(GM_LESS_DARKER_RED, 125, 0, 0);
	init_color(GM_MORE_DARKER_RED, 46, 9, 8);
	init_color(GM_LESS_DARKER_GREEN, 0, 120, 0);
	init_color(GM_MORE_DARKER_GREEN, 0, 80, 0);

	init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(RED_YEL, COLOR_RED, COLOR_YELLOW);
	init_pair(WHI_RED, COLOR_WHITE, COLOR_RED);
	init_pair(WHI_LRED, COLOR_WHITE, GM_LESS_DARKER_RED);
	init_pair(WHI_DRED, COLOR_WHITE, GM_MORE_DARKER_RED);

	init_pair(WHI_BL, COLOR_WHITE, COLOR_BLUE);
	init_pair(WHI_GR, COLOR_WHITE, COLOR_GREEN);
	init_pair(GRE_BL, COLOR_GREEN, COLOR_BLACK);
	init_pair(RED_BL, COLOR_RED, COLOR_BLACK);
	init_pair(WHI_LGR, COLOR_WHITE, GM_LESS_DARKER_GREEN);
	init_pair(WHI_DGR, COLOR_WHITE, GM_MORE_DARKER_GREEN);
	init_color(CYAN, 0, 174, 177);
	init_pair(CY_BL, CYAN, COLOR_BLACK);
	init_pair(YEL_BL, COLOR_YELLOW, COLOR_BLACK);
	wbkgd(w1,COLOR_PAIR(YEL_BL));

	/* Creazione indicatore e coordinate per il menù */
	Posizione oggetto = { (maxx/2)-12,(maxy/2)+1 };
	mvwprintw(w1,oggetto.y,oggetto.x,"-");
	int ymod=(maxy/2)-5; //Ordinate scritta indicativa
	int yext=(maxy/2)+1; //Ordinate scritta EXIT
	int ygov=(maxy/2)+2; //Ordinate scritta Game-Over
	int yflscr=(maxy/2)+3; //Ordinate scritta Schermo COmpleto
	int ythdr=(maxy/2)+4; //Ordinate scritta Schermo COmpleto
	int xbase=(maxx/2)-10; //Ascisse di default



	/* Selezione menù */
	while (true) {
			/* Creazioni grafiche per il menù */
			box(w1,ACS_VLINE,ACS_HLINE);
			mvwprintw(w1,ymod,xbase,"Inserire una modalità:");
			mvwprintw(w1,yext,xbase,"EXIT");
			mvwprintw(w1,ygov,xbase,"Game-Over");
			mvwprintw(w1,yflscr,xbase,"Versione processi");
			mvwprintw(w1,ythdr,xbase,"Versione threads");
			wrefresh(w1);

			/* Opzione tasti di movimento e selezione */
			key = wgetch(w1);
		
			switch(key) {
			      	case 65: //Freccia verso l'alto
					if(oggetto.y == yext){ //Ritorno dalla prima all'ultima opzione
						oggetto.y=yflscr;
					}
					else{ 
						if(oggetto.y > 0){
							oggetto.y -= 1; //Aggiornamento ordinata dell'indicatore
						}
					}
					break;

				case 66: //Freccia verso il basso
					if(oggetto.y == yflscr){ //Ritorno dall'ultima alla prima opzione
						oggetto.y=yext;
					}
					else{ 
						if(oggetto.y < maxy - 1){
							oggetto.y += 1; //Aggiornamento ordinata dell'indicatore
						}
					}
					break;

				case 10: //Tasto invio
					if(oggetto.y == yext){ //Scelta menù = EXIT
				     	endwin();
						exit(0);
					}
					if(oggetto.y == ygov){ //Scelta menù = Animazione Game Over
						wclear(w1); 
						game_over(w1,maxx/2,maxy/2);
						wclear(w1);
						endwin();
						exit(0);
					}
					if(oggetto.y == yflscr){ //Scelta menù = Animazione completa
						screen(w1);
						wclear(w1);
						endwin();
						exit(0);
					}
					break; 
				}

		/* Spostamento dell'indicatore in base al tasto premuto */
		wclear(w1);
		mvwaddch(w1,oggetto.y, oggetto.x, '-');
		}
}
