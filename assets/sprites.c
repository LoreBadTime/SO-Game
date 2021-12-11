
#include "sprites.h"

void printnavetta(int x,int y,WINDOW *w){
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
}
void printnemicolv2(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]="O O";
	sprite.c[1]=" X ";
	sprite.c[2]="O O";
	mvwprintw(w,y-1,x-1,sprite.c[0]);
	mvwprintw(w,y,x-1,sprite.c[1]);
	mvwprintw(w,y+1,x-1,sprite.c[2]);
}
void printnemicolv1(int x,int y,WINDOW *w){
	oggetto sprite;
	int i,k;
	sprite.c[0]=" /O";
	sprite.c[1]="-X ";
	sprite.c[2]=" \\O";
	mvwprintw(w,y-1,x-1,sprite.c[0]);
	mvwprintw(w,y,x-1,sprite.c[1]);
	mvwprintw(w,y+1,x-1,sprite.c[2]);
}

void print_vita(int life,WINDOW *w1){
    int i = 0;
	while (i < life)
	{
		mvwaddch(w1, 0, 3 + i, '>');
		++i;
	}
}

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
