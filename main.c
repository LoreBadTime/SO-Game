#include "./globalincludes.h"
#include "./assets/animations.h"


void main(int mode){
    //necessario un menu di debug
    mode = 1;
    initscr();
    start_color();
    noecho();
    curs_set(0);
    cbreak();
    //test momentaneo in attesa di un menu di debug
    if(mode == 1){
        game_over(1,11);
    }
    
    refresh();
    endwin();
}
