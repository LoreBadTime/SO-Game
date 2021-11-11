#include "./globalincludes.h"
#include "./assets/animations.h"


typedef enum {
	EXIT, GAME_OVER, ENEMY_LV1,TEST
} Mode;

struct posizione {
  int x;        // coordinata x
  int y;        // coordinata y
};


void menu();

void main(){
    menu();
}

void menu(){
	int maxx=0;
	int maxy=0;
	int mode=-1;
	int key=0;
	int enter='\n';
	WINDOW *w1;

	initscr();
	noecho();
	keypad(stdscr, 1);
	curs_set(0);

	getmaxyx(stdscr, maxy, maxx);
	w1 = newwin (maxy,maxx,0,0);

	struct posizione oggetto = { (maxx/2)-12, (maxy/2)+1 };
	/* Prima visualizzazione al centro */
	mvwprintw(w1,oggetto.y,oggetto.x,"-");

	while (true) {
			box(w1,ACS_VLINE,ACS_HLINE);
			mvwprintw(w1,(maxy/2)-5,(maxx/2)-10,"Inserire una modalità:");
			mvwprintw(w1,(maxy/2)+1,(maxx/2)-10,"EXIT");
			mvwprintw(w1,(maxy/2)+2,(maxx/2)-10,"Game-Over");
			mvwprintw(w1,(maxy/2)+3,(maxx/2)-10,"Schermo completo");
			wrefresh(w1);
			key = wgetch(w1);
		
			switch(key) {
			      case 65:
				if(oggetto.y == (maxy/2)+1){oggetto.y=(maxy/2)+3;}
				else{ if(oggetto.y > 0) oggetto.y -= 1; }
			       break;
			      case 66:
				if(oggetto.y == (maxy/2)+3){oggetto.y=(maxy/2)+1;}
				else{ if(oggetto.y < maxy - 1) oggetto.y += 1; }
			       break;
				case 10:
				if(oggetto.y == (maxy/2)+1){
		     		endwin();
				exit(0);}
				if(oggetto.y == (maxy/2)+2){ wclear(w1);
				game_over(w1,1,11);
				wclear(w1);
				endwin();
				exit(0);}
				if(oggetto.y == (maxy/2)+3) screen();
			}

		wclear(w1);
		mvwaddch(w1,oggetto.y, oggetto.x, '-');
	}
}
