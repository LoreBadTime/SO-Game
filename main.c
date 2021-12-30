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
    int y_NUMNEMICI = (maxy / 2);
    int y_COLORE = (maxy / 2) + 2;
    int y_RIMBALZI = (maxy / 2) + 4;
    int y_EXITSETTINGS = (maxy / 2) + 8;
    int settings; // Flag per attivare il while delle settings
    int numnemici = 5;
    int colore = 0;
    int rimbalzi = 0;
    char *rimbalzi_ON = "ON";
    char *rimbalzi_OFF = "OFF";
    char *bianco = "BIANCO";
    char *ciano = "CIANO";
    char *rosso = "ROSSO";
    char *verde = "VERDE";
    char *giallo = "GIALLO";
    int i = 0;

    int xbase = (maxx / 2) - 10 + fixscreen; //Ascisse di default
    Posizione oggetto = {(maxx / 2) - 12 + fixscreen,y_START };



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
                        oggetto.y = y_SETTINGS;
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
                    screen(w1,numnemici,rimbalzi,colore);
                    wclear(w1);
                }
                if (oggetto.y == y_SETTINGS) { //Scelta menù = Impostazioni di gioco
                    settings = 1;
                    oggetto.y = y_NUMNEMICI;
                    while (settings) {
                        wclear(w1);
                        box(w1, ACS_VLINE, ACS_HLINE);
                        wbkgd(w1, COLOR_PAIR(GRE_BL));
                        wattron(w1, COLOR_PAIR(WHITE_BLACK));
                        mvwprintw(w1, oggetto.y, oggetto.x - 2, "-->");
                        wattroff(w1, COLOR_PAIR(WHITE_BLACK));
                        wattron(w1, COLOR_PAIR(RED_BL));
                        mvwprintw(w1, y_NUMNEMICI-3, xbase - 20 , "Attenzione: Le modifiche vengono salvate automaticamente.");
                        wattroff(w1, COLOR_PAIR(RED_BL));
                        mvwprintw(w1, y_NUMNEMICI, xbase, "Selezionare numero nemici:");
                        wattron(w1, COLOR_PAIR(WHITE_BLACK));
                        mvwprintw(w1, y_NUMNEMICI, xbase + 30, "%d",numnemici);
                        wattroff(w1, COLOR_PAIR(WHITE_BLACK));
                        mvwprintw(w1, y_COLORE, xbase, "Selezionare colore nave:");
                        if(colore == 0) { //BIANCO
                            wattron(w1, COLOR_PAIR(WHITE_BLACK));
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", bianco);
                            printnavetta(maxx/5,3*maxy/5,w1);
                            wattroff(w1, COLOR_PAIR(WHITE_BLACK));
                        }
                        if(colore == 1) { //CIANO
                            wattron(w1, COLOR_PAIR(CY_BL));
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", ciano);
                            printnavetta(maxx/5,3*maxy/5,w1);
                            wattroff(w1, COLOR_PAIR(CY_BL));
                        }
                        if(colore == 2) { //ROSSO
                            wattron(w1, COLOR_PAIR(RED_BL));
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", rosso);
                            printnavetta(maxx/5,3*maxy/5,w1);
                            wattroff(w1, COLOR_PAIR(RED_BL));
                        }
                        if(colore == 3) { //VERDE
                            wattron(w1, COLOR_PAIR(GRE_BL));
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", verde);
                            printnavetta(maxx/5,3*maxy/5,w1);
                            wattroff(w1, COLOR_PAIR(GRE_BL));
                        }
                        if(colore == 4) { //GIALLO
                            wattron(w1, COLOR_PAIR(YEL_BL));
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", giallo);
                            printnavetta(maxx/5,3*maxy/5,w1);
                            wattroff(w1, COLOR_PAIR(YEL_BL));
                        }
                        mvwprintw(w1, y_RIMBALZI, xbase, "Rimbalzi tra nemici:");
                        if(rimbalzi == 1) {
                            wattron(w1, COLOR_PAIR(GRE_BL));
                            mvwprintw(w1, y_RIMBALZI, xbase + 30, "%s", rimbalzi_ON);
                            wattroff(w1, COLOR_PAIR(GRE_BL));
                            wattron(w1, COLOR_PAIR(WHITE_BLACK));
                            for(i=1;i<numnemici+1;i++){
                                printnemicolv1_f1(6*i ,maxy/5,w1);
                            }
                            wattroff(w1, COLOR_PAIR(WHITE_BLACK));
                        }
                        if(rimbalzi == 0) {
                            wattron(w1, COLOR_PAIR(RED_BL));
                            mvwprintw(w1, y_RIMBALZI, xbase + 30, "%s", rimbalzi_OFF);
                            wattroff(w1, COLOR_PAIR(RED_BL));
                            wattron(w1, COLOR_PAIR(WHITE_BLACK));
                            for(i=1;i<numnemici+1;i++){
                                printnemicolv1_f2(6*i ,maxy/5,w1);
                            }
                            wattroff(w1, COLOR_PAIR(WHITE_BLACK));
                        }
                        mvwprintw(w1, y_EXITSETTINGS, xbase + 10, "EXIT");

                        key = wgetch(w1);
                        switch (key) {
                            case 66: // Freccia verso il basso
                                if (oggetto.y == y_EXITSETTINGS) {
                                    oggetto.y = y_NUMNEMICI;
                                    oggetto.x = xbase - 2;
                                } else {
                                    if (oggetto.y == y_RIMBALZI) {
                                        oggetto.y = y_EXITSETTINGS;
                                        oggetto.x = xbase + 8;
                                    } else {
                                        oggetto.y += 2;
                                    }
                                }
                                break;
                            case 65: // Freccia verso l'alto
                                if (oggetto.y == y_NUMNEMICI) {
                                    oggetto.y = y_EXIT;
                                    oggetto.x = xbase + 8;
                                } else {
                                    if (oggetto.y == y_EXIT) {
                                        oggetto.y = y_RIMBALZI;
                                        oggetto.x = xbase - 2;
                                    } else {
                                        oggetto.y -= 2;
                                    }
                                }
                                break;
                            case 67: // Freccia verso destra
                                if (oggetto.y == y_NUMNEMICI) {
                                    if(numnemici != ENEM_TEST) {
                                        numnemici++;
                                    } else {
                                        numnemici = 0;
                                    }
                                }
                                if (oggetto.y == y_COLORE) {
                                    if(colore != 4) { //Ultimo colore selezionabile (GIALLO)
                                        colore++;
                                    }
                                    else {
                                        colore = 0;
                                    }

                                }
                                if (oggetto.y == y_RIMBALZI) {
                                    if (rimbalzi == 0) {
                                        rimbalzi = 1;
                                    } else {
                                        rimbalzi = 0;
                                    }
                                }
                                break;
                            case 68: // Freccia verso sinistra
                                if (oggetto.y == y_NUMNEMICI) {
                                    if(numnemici != 0) {
                                        numnemici--;
                                    } else {
                                        numnemici = ENEM_TEST;
                                    }
                                }
                                if (oggetto.y == y_COLORE) {
                                    if (colore != 0) {
                                        colore--;
                                    } else {
                                        colore = 4; //Ultimo colore selezionabile (GIALLO)
                                    }
                                }
                                if (oggetto.y == y_RIMBALZI) {
                                    if (rimbalzi == 1) {
                                        rimbalzi = 0;
                                    } else {
                                        rimbalzi = 1;
                                    }
                                }
                                break;
                            case 10: // Tasto invio
                                if (oggetto.y == y_EXIT) {
                                    settings = 0;
                                }
                                break;
                        }
                        xbase = (maxx / 2) - 10 + fixscreen; //Ascisse di default
                    }
                    /* Reset valori per il ritorno al menù */
                    oggetto.x = (maxx / 2) - 12 + fixscreen;
                    oggetto.y = y_SETTINGS;
                }
                break;
        }

        /* Spostamento dell'indicatore in base al tasto premuto */
        wclear(w1);
    }
}
