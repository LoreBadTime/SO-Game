#include "sprites.h"
//stampa navetta in generale
void printnavetta(int x,int y,WINDOW *w){

	wattron(w,COLOR_PAIR(CY_BL));
	oggetto sprite;
	int i,k;
	sprite.c[0]="|\\ ";
	sprite.c[1]="| \\ ";
	sprite.c[2]="|-O|==";
	sprite.c[3]="| /";
	sprite.c[4]="|/";
	for(i=(-2),k=0;k<5;i++,k++){
	mvwprintw(w,y+i,x-5,sprite.c[k]);
	}
	wattroff(w,COLOR_PAIR(CY_BL));
}
//animazioni navetta gameover (frame 1)
void printnavetta_distrutta_f1(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="|X ";
	sprite.c[1]="|X\\ ";
	sprite.c[2]="X-O|X=";
	sprite.c[3]="| X";
	sprite.c[4]="X/";
	for(i=(-2),k=0;k<5;i++,k++){
	mvwprintw(w,y+i,x-5,sprite.c[k]);
	}
}
//animazioni navetta gameover (frame 2)
void printnavetta_distrutta_f2(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="|\\ ";
	sprite.c[1]="X X ";
	sprite.c[2]="|-X|=X";
	sprite.c[3]="|X/";
	sprite.c[4]="X/";
	for(i=(-2),k=0;k<5;i++,k++){
	mvwprintw(w,y+i,x-5,sprite.c[k]);
	}
}
//animazioni navetta gameover (frame 3)
void printnavetta_distrutta_f3(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="|\\ ";
	sprite.c[1]="     ";
	sprite.c[2]="|- |=   ";
	sprite.c[3]="| /";
	sprite.c[4]=" /";
	for(i=(-2),k=0;k<5;i++,k++){
	mvwprintw(w,y+i,x-5,sprite.c[k]);
	}
}
//animazioni navetta gameover (frame 4)
void printnavetta_distrutta_f4(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="|   ";
	sprite.c[1]="    ";
	sprite.c[2]=" - |  ";
	sprite.c[3]="|   ";
	sprite.c[4]="   ";
	for(i=(-2),k=0;k<5;i++,k++){
	mvwprintw(w,y+i,x-5,sprite.c[k]);
	}
}
//animazione nemico lv 2 di template per collisioni frame 1
void printnemicolv2_f1(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="O-O";
	sprite.c[1]="/V\\";
	sprite.c[2]="O-O";
	mvwprintw(w,y-1,x,sprite.c[0]);
	mvwprintw(w,y,x,sprite.c[1]);
	mvwprintw(w,y+1,x,sprite.c[2]);
}
//animazione nemico lv 2 di template per collisioni frame 2
void printnemicolv2_f2(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="O-O";
	sprite.c[1]="|X| ";
	sprite.c[2]="O-O";
	mvwprintw(w,y-1,x,sprite.c[0]);
	mvwprintw(w,y,x,sprite.c[1]);
	mvwprintw(w,y+1,x,sprite.c[2]);
}

//animazione nemico lv 2 di template per collisioni frame 3
void printnemicolv2_f3(int x,int y,WINDOW *w){
    oggetto sprite;
    int i,k;
    sprite.c[0]="O X";
    sprite.c[1]=" X ";
    sprite.c[2]="X O";
    mvwprintw(w,y-1,x,sprite.c[0]);
    mvwprintw(w,y,x,sprite.c[1]);
    mvwprintw(w,y+1,x,sprite.c[2]);
}

//animazione nemico lv 2 di template per collisioni frame 4
void printnemicolv2_f4(int x,int y,WINDOW *w){
    oggetto sprite;
    int i,k;
    sprite.c[0]="X O";
    sprite.c[1]=" X ";
    sprite.c[2]="O X";
    mvwprintw(w,y-1,x,sprite.c[0]);
    mvwprintw(w,y,x,sprite.c[1]);
    mvwprintw(w,y+1,x,sprite.c[2]);
}


//animazione nemico lv 1 di template per collisioni frame 1
void printnemicolv1_f1(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]=" /O";
	sprite.c[1]="-O ";
	sprite.c[2]=" \\O";
	mvwprintw(w,y-1,x,sprite.c[0]);
	mvwprintw(w,y,x,sprite.c[1]);
	mvwprintw(w,y+1,x,sprite.c[2]);
}
//animazione nemico lv 1 di template per collisioni frame 2
void printnemicolv1_f2(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]=" \\O";
	sprite.c[1]="-O ";
	sprite.c[2]=" /O";
	mvwprintw(w,y-1,x,sprite.c[0]);
	mvwprintw(w,y,x,sprite.c[1]);
	mvwprintw(w,y+1,x,sprite.c[2]);
}

//Stampa numero di vite e disponibilita proiettili a schermo
void print_info(int proiettile_pronto,int life,WINDOW *w1,int maxx){
    int i;
    mvwprintw(w1, 0, 1, "Proiettile:" );
    if(proiettile_pronto == PRONTO)
        mvwaddch(w1, 0, 13, ' ');
    else
        mvwaddch(w1,0,13,'O');

    mvwprintw(w1, 0 , 16, "Vite:");
	for (i=0;i < life;i++){
		mvwaddch(w1, 0, 22 + i, '#');
	}

    for(i=0;i<maxx;i++){
        mvwaddch(w1,1,i,'_');
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
// test sprites
void main_test(){
	initscr();
	noecho();
	curs_set(0);
	int maxx =0, maxy = 0, i = 0;
	oggetto sprite;
	sprite.c[0]="|\\ ";
	sprite.c[1]="| \\ ";
	sprite.c[2]="|  |==";
	sprite.c[3]="| /";
	sprite.c[4]="|/";
	char c;

	getmaxyx(stdscr, maxy, maxx);
	sprite.x = 1, sprite.y = (maxy/2);
	
	for(i=0;i<DIM;i++) mvprintw(sprite.y+i,sprite.x,sprite.c[i]);
	refresh();

	do{
		c=getch();
		switch(c){
			case SU:
				if(sprite.y!=0){
					sprite.y--;
					clear();
					for(i=0;i<DIM;i++)
					mvprintw(sprite.y+i,sprite.x,sprite.c[i]);	
					refresh();
				}
				break;
			case GIU:
				if(sprite.y+5!=maxy){
					sprite.y++;
					clear();
					for(i=0;i<DIM;i++)
					mvprintw(sprite.y+i,sprite.x,sprite.c[i]);	
					refresh();
				}
				break;		
		}
	} while(!(c == 'q' || c=='Q'));
	
	endwin();
}
