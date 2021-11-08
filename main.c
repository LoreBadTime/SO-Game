
#include "./globalincludes.h"
#include "./assets/animations.h"

typedef enum {
	EXIT, GAME_OVER, ENEMY_LV1
} Mode;

void menu();

void main(){
    menu();
}


void menu(){
	int mode=-1;

	do{
	
	printf("\nInserire una modalità:"
		"\n0) EXIT"
		"\n1) Game-Over"
        "\n2) Test nemici multipli lv1"
		"\n\nScelta:");
	scanf("%d",&mode);

    initscr();
	start_color();
	noecho();
	curs_set(0);
	cbreak();

	switch (mode) {
		case EXIT:
			mode=0;
		break;
        
        case ENEMY_LV1:
        //standard screen,da piu controllo
			screen();
        break;

		case GAME_OVER:
			game_over(1,11);
		break;
		}
        
    refresh();
	endwin();
	}while (mode!=EXIT);

	printf("\nProgramma terminato.\n\n");
}
