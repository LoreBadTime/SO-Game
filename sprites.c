#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define SU 65 /* Freccia su */
#define GIU 66 /* Freccia giu */
#define DIM 5

typedef struct {
	int x;
	int y;
	char* c[DIM-1];
} oggetto;

void printnavetta(int x,int y,WINDOW *w){
	oggetto sprite;
	sprite.c[0]="|\\ ";
	sprite.c[1]="| \\ ";
	sprite.c[2]="|  |==";
	sprite.c[3]="| /";
	sprite.c[4]="|/";
	for(i=(-2),k=0;k<5;i++,k++){
	mvwprintw(w,y+i,x,sprite.c[k]);
	}
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
