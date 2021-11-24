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
	char c;
} oggetto;

void main(){
	initscr();
	noecho();
	curs_set(0);
	int maxx =0, maxy = 0, i = 0;
	char* sprite[DIM-1];
	sprite[0]="|\\ ";
	sprite[1]="| \\ ";
	sprite[2]="|  |==";
	sprite[3]="| /";
	sprite[4]="|/";
	char c;

	getmaxyx(stdscr, maxy, maxx);
	int x = 1, y = (maxy/2);
	
	for(i=0;i<DIM;i++) mvprintw(y+i,x,sprite[i]);
	refresh();

	do{
		c=getch();
		switch(c){
			case SU:
				if(y!=0){
					y--;
					clear();
					for(i=0;i<DIM;i++)
					mvprintw(y+i,x,sprite[i]);	
					refresh();
				}
				break;
			case GIU:
				if(y+5!=maxy){
					y++;
					clear();
					for(i=0;i<DIM;i++)
					mvprintw(y+i,x,sprite[i]);	
					refresh();
				}
				break;		
		}
	} while(!(c == 'q' || c=='Q'));
	
	endwin();
}
