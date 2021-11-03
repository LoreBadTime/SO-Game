#include "animations.h"
//velocita e imppostazione animazione


//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori

//animazione di gameover,x prende le ascisse della navetta attuali(potrebbe non servire),y le ordinate
void game_over(int x,int y){
    clear();
    //print sprite with x and y
    //clear unused processes
    refresh();
    //definizione colori
    const char string[] = "GAME OVER";
    init_pair(1,COLOR_RED,COLOR_YELLOW);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_color(COLOR_BLACK,0, 0, 0);
    init_color(GM_LESS_DARKER_RED,125, 0, 0);
    init_color(GM_MORE_DARKER_RED,46, 9, 8);
    init_pair(3,COLOR_WHITE,GM_LESS_DARKER_RED);
    init_pair(4,COLOR_WHITE,GM_MORE_DARKER_RED);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);
    int i = 0;
    //animazione schermo che flasha
    while(i < GM_CYCLES){
        mvprintw(y,x,NAVE_OK);
        bkgd(COLOR_PAIR(1));
        refresh();
        napms(GM_SPEED);
        clear();
        mvprintw(y,x,NAVE_ESPLOSA);
        bkgd(COLOR_PAIR(2));
        refresh();
        napms(GM_SPEED);
        clear();
        i++;
    }
    //morte lenta
    clear();
    mvprintw(y,x,NAVE_OK);
    napms(GM_DEATH);

    mvprintw(y,x,NAVE_ESPLOSA);
    refresh();
    napms(GM_DEATH);

    clear();
    mvprintw(y,x,NAVE_ESPLOSA);
    bkgd(COLOR_PAIR(3));
    refresh();
    napms(GM_DEATH-GM_SPEED);

    clear();
    mvprintw(y,x,NAVE_ESPLOSA);
    bkgd(COLOR_PAIR(4));
    refresh();
    napms(GM_DEATH-GM_SPEED);

    clear();
    mvprintw(y,x,NAVE_ESPLOSA);
    bkgd(COLOR_PAIR(5));
    refresh();
    napms(7*GM_SPEED);

    clear();
    refresh();
    sleep(5);

    i = 0;
    x = LINES;
    y = COLS;
    //mwprintw accepts only strings,so i need to convert single char into strings to make the animation
    char tmp[2] = {' ', '\0'};
    while (i < GM_CYCLES2){
        tmp[1] = string[i];
        mvprintw(x/2,(y/2+i)-5,tmp);
        refresh();
        napms(3*GM_SPEED);
        i++;
    }
    clear();
    refresh();
    napms(3*GM_SPEED);
    mvprintw(x/2,(y/2)-5,string);
    refresh();
    sleep(10);
    //per fare prima si può riportare tutto indietro al menù principale
}