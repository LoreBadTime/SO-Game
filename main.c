#include "./globalincludes.h"
#include "./assets/animations.h"

typedef enum {
	EXIT, GAME_OVER
} Mode;

void menu();

void main(){
    menu();
}

void menu(){
	int mode=0;

	do{
	
	printf("\nInserire una modalità:"
		"\n0) EXIT"
		"\n1) Game-Over"
		"\n\nScelta:");
	scanf("%d",&mode);

	switch (mode) {
		case EXIT:
			mode=0;
		break;

		case GAME_OVER:
			initscr();
			start_color();
			noecho();
			curs_set(0);
			cbreak();
			game_over(1,11);
			refresh();
			endwin();
		break;
		}

	}while (mode!=EXIT);

	printf("\nProgramma terminato.\n\n");
}
