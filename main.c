#include "./globalincludes.h"

void main(){
	

	menu();
}

void menu() {
    /* Variabili */
    int maxx = 0;
    int maxy = 0;
    int key = 0;
    int fixscreen = 0;
    WINDOW *w1;

    /* Inizializzazione schermo */
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, 1);


    /* Personalizzazione colori e dimensioni dello schermo */

    //
    getmaxyx(stdscr, maxy, maxx);
    //risolvere problemi nel caso di risoluzione dispari
    if(maxx % 2 == 1){
        --maxx;
        ++fixscreen;
    }
    w1 = newwin(maxy, maxx, 0, 0);
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
    init_pair(CY_BLU, CYAN, COLOR_BLUE);
    init_pair(CY_GRE, CYAN, COLOR_GREEN);
    init_pair(DARKRED_BL, GM_LESS_DARKER_RED, COLOR_BLACK);

    /* Creazione indicatore e coordinate per il menù */
    int y_SELECT = (maxy / 2) + 2 + fixscreen; //Ordinate scritta indicativa
    int y_START = (maxy / 2) + 5 + fixscreen; //Ordinate scritta Inizio partita
    int y_SETTINGS = (maxy / 2) + 6 + fixscreen; //Ordinate scritta EXIT
    int y_EXIT = (maxy / 2) + 8 + fixscreen; //Ordinate scritta EXIT
    int xbase = (maxx / 2) - 10 + fixscreen; //Ascisse di default
    Posizione oggetto = {(maxx / 2) - 12 + fixscreen, y_START};



    /* Selezione menù */
    while (true) {
        wattron(w1,COLOR_PAIR(WHITE_BLACK));
        mvwprintw(w1, oggetto.y, oggetto.x-2, "-->");
        wattroff(w1,COLOR_PAIR(WHITE_BLACK));
        /* Creazioni grafiche per il menù */
        box(w1, ACS_VLINE, ACS_HLINE);
        wbkgd(w1, COLOR_PAIR(YEL_BL));
        wattron(w1,COLOR_PAIR(RED_BL));
        print_titolo(w1, (xbase-14),(y_START-15));
        wattroff(w1,COLOR_PAIR(RED_BL));
        wattron(w1,COLOR_PAIR(GRE_BL));
        mvwprintw(w1, y_SELECT, xbase-5, "\\_|_/ SELEZIONARE UN OPZIONE \\_|_/");
        wattroff(w1,COLOR_PAIR(GRE_BL));
        mvwprintw(w1, y_START, xbase, "Inizia una partita");
        mvwprintw(w1, y_SETTINGS, xbase, "Impostazioni di gioco");
        mvwprintw(w1, y_EXIT, xbase, "EXIT");
        wrefresh(w1);


        /* Opzione tasti di movimento e selezione */
        key = wgetch(w1);

        switch (key) {
            case 65: //Freccia verso l'alto
                if (oggetto.y == y_START) { //Ritorno dalla prima all'ultima opzione
                    oggetto.y = y_EXIT;
                } else {
                    if (oggetto.y == y_EXIT){
                        oggetto.y -= 2;
                    } else if (oggetto.y > 0) {
                        oggetto.y -= 1; //Aggiornamento ordinata dell'indicatore
                    }
                }
                break;

            case 66: //Freccia verso il basso
                if (oggetto.y == y_EXIT) { //Ritorno dall'ultima alla prima opzione
                    oggetto.y = y_START;
                } else {
                    if (oggetto.y == y_SETTINGS){
                        oggetto.y += 2;
                    } else if (oggetto.y < maxy - 1) {
                        oggetto.y += 1; //Aggiornamento ordinata dell'indicatore
                    }
                }
                break;

            case 10: //Tasto invio
                if (oggetto.y == y_EXIT) { //Scelta menù = EXIT
                    endwin();
                    exit(0);
                }
                if (oggetto.y == y_START) { //Scelta menù = Animazione completa
                    screen(w1);
                    wclear(w1);
                }
                if (oggetto.y == y_SETTINGS) { //Scelta menù = Impostazioni di gioco
                    //screen(w1);
                    //wclear(w1);
                }
                break;
        }

        /* Spostamento dell'indicatore in base al tasto premuto */
        wclear(w1);
    }
}
