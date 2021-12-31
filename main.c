#include "./globalincludes.h"

void main(){
	menu();
}

void menu() {
    
    /* Inizializzazione schermo */
    int maxx = 0; // Variabile risoluzione per ottenere l'ascissa massima
    int maxy = 0; // Variabile risoluzione per ottenere l'ordinata massima
    int key = 0; // Tasto premuto dal giocatore
    int fixscreen = 0; // Variabile per fixare lo schermo in caso di risoluzione dispari
    WINDOW *w1; // Finestra principale di gioco
    initscr(); // Inizializzazione dello schermo
    noecho(); // I caratteri non sono mostrati
    curs_set(0); // Nascondi il cursore
    keypad(stdscr, 1); // Abilitazione keypad all'interno dello schermo
    
    /* Personalizzazione colori e dimensioni dello schermo */
    getmaxyx(stdscr, maxy, maxx); // Ottenimento delle risoluzioni massime
    if(maxx % 2 == 1){ // Nel caso la risoluzione sia dispari, adatta lo schermo
        --maxx;
        ++fixscreen;
    }
    w1 = newwin(maxy, maxx, 0, 0); // Inizializza la nuova finestra con le coordinate desiderate
    start_color();
    
    /* Definizione colori */
    init_color(COLOR_BLACK, 0, 0, 0); // Colore: NERO
    init_color(GM_LESS_DARKER_RED, 125, 0, 0); // Colore: ROSSO CHIARO
    init_color(GM_MORE_DARKER_RED, 46, 9, 8); // Colore: ROSSO SCURO
    init_color(GM_LESS_DARKER_GREEN, 0, 120, 0); // Colore: VERDE CHIARO
    init_color(GM_MORE_DARKER_GREEN, 0, 80, 0); // Colore: VERDE SCURO
    init_color(CYAN, 0, 174, 177); // Colore: CIANO

    /* Definizione carattere e sfondo */
    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK); // Carattere: BIANCO, Sfondo: NERO
    init_pair(WHI_LRED, COLOR_WHITE, GM_LESS_DARKER_RED); // Carattere: BIANCO, Sfondo: ROSSO CHIARO
    init_pair(WHI_DRED, COLOR_WHITE, GM_MORE_DARKER_RED); // Carattere: BIANCO, Sfondo: ROSSO SCURO
    init_pair(WHI_LGR, COLOR_WHITE, GM_LESS_DARKER_GREEN); // Carattere: BIANCO, Sfondo: VERDE CHIARO
    init_pair(WHI_DGR, COLOR_WHITE, GM_MORE_DARKER_GREEN); // Carattere: BIANCO, Sfondo: VERDE SCURO
    init_pair(WHI_RED, COLOR_WHITE, COLOR_RED); // Carattere: BIANCO, Sfondo: ROSSO
    init_pair(WHI_BL, COLOR_WHITE, COLOR_BLUE); // Carattere: BIANCO, Sfondo: BLU
    init_pair(WHI_GR, COLOR_WHITE, COLOR_GREEN); // Carattere: BIANCO, Sfondo: VERDE
    init_pair(GRE_BL, COLOR_GREEN, COLOR_BLACK); // Carattere: VERDE, Sfondo: NERO
    init_pair(RED_BL, COLOR_RED, COLOR_BLACK); // Carattere: ROSSO, Sfondo: NERO
    init_pair(RED_YEL, COLOR_RED, COLOR_YELLOW); // Carattere: ROSSO, Sfondo: GIALLO
    init_pair(CY_BL, CYAN, COLOR_BLACK); // Carattere: CIANO, Sfondo: NERO
    init_pair(YEL_BL, COLOR_YELLOW, COLOR_BLACK); // Carattere: GIALLO, Sfondo: NERO
    init_pair(CY_BLU, CYAN, COLOR_BLUE); // Carattere: CIANO, Sfondo: BLU
    init_pair(CY_GRE, CYAN, COLOR_GREEN); // Carattere: CIANO, Sfondo: VERDE
    init_pair(DARKRED_BL, GM_LESS_DARKER_RED, COLOR_BLACK); // Carattere: ROSSO SCURO, Sfondo: NERO

    /* Flag e coordinate per i menù */
    int i; // Contatore
    int y_SELECT = (maxy / 2) + 2 + fixscreen; // Ordinate scritta menu principale: Indicativa
    int y_START = (maxy / 2) + 5 + fixscreen; // Ordinate scritta menu principale: START
    int y_SETTINGS = (maxy / 2) + 6 + fixscreen; // Ordinate scritta menu principale: SETTINGS
    int y_EXIT = (maxy / 2) + 8 + fixscreen; // Ordinate scritta menu principale: EXIT
    int y_NUMNEMICI = (maxy / 2); // Ordinate scritta menu settings: NUMERO NEMICI
    int y_COLORE = (maxy / 2) + 2; // Ordinate scritta menu settings: COLORI
    int y_RIMBALZI = (maxy / 2) + 4; // Ordinate scritta menu settings: RIMBALZI
    int y_EXITSETTINGS = (maxy / 2) + 8; // Ordinate scritta menu settings: EXIT
    int settings; // Flag per attivare il ciclo delle settings
    int numnemici = 5; // Selezione dei nemici nel menu settings
    int colore = 0; // Selezione del colore nel menu settings
    int rimbalzi = 0; // Selezione dei rimbalzi nel menu settings
    int xbase = (maxx / 2) - 10 + fixscreen; // Ascisse di default per i menu
    Posizione oggetto = {(maxx / 2) - 12 + fixscreen,y_START }; // Puntatore dei menù
    
    /* Stringhe visualizzate */
    char *rimbalzi_ON = "ON"; // Stringa per i rimbalzi
    char *rimbalzi_OFF = "OFF"; // Stringa per i rimbalzi
    char *bianco = "BIANCO"; // Stringa per i colori
    char *ciano = "CIANO"; // Stringa per i colori
    char *rosso = "ROSSO"; // Stringa per i colori
    char *verde = "VERDE"; // Stringa per i colori
    char *giallo = "GIALLO"; // Stringa per i colori


    /* Menù principale */
    while (true) {

        /* Creazioni grafiche per il menù */
        /* Nei commenti, il primo colore della coppia si riferisce al carattere, il secondo colore allo schermo */
        box(w1, ACS_VLINE, ACS_HLINE); // Crea una box all'interno della finestra di gioco
        wbkgd(w1, COLOR_PAIR(YEL_BL)); // Sfondo: GIALLO / NERO
        
        wattron(w1,COLOR_PAIR(WHITE_BLACK)); // Start: BIANCO / NERO
        mvwprintw(w1, oggetto.y, oggetto.x-2, "-->"); // Stampa del puntatore del menù
        wattroff(w1,COLOR_PAIR(WHITE_BLACK)); // End: BIANCO / NERO
        
        wattron(w1,COLOR_PAIR(RED_BL)); // Start: ROSSO / NERO
        print_titolo(w1, (xbase-14),(y_START-15)); // Stampa del titolo "Space Defender"
        wattroff(w1,COLOR_PAIR(RED_BL)); // End: ROSSO / NERO
        
        wattron(w1,COLOR_PAIR(GRE_BL)); // Start: VERDE / NERO
        mvwprintw(w1, y_SELECT, xbase-5, "\\_|_/ SELEZIONARE UN OPZIONE \\_|_/"); // Stampa indicativa
        wattroff(w1,COLOR_PAIR(GRE_BL)); // End: VERDE / NERO
        
        /* Stampe delle opzioni disponibili nel menu principale */
        mvwprintw(w1, y_START, xbase, "Inizia una partita");
        mvwprintw(w1, y_SETTINGS, xbase, "Impostazioni di gioco");
        mvwprintw(w1, y_EXIT, xbase, "EXIT");
        wrefresh(w1); // Refresh della finestra


        /* Opzione tasti di movimento e selezione */
        key = wgetch(w1); // Assegna il carattere digitato alla variabile
        switch (key) {
            case 65: // Se il tasto premuto è: Freccia verso l'alto
                if (oggetto.y == y_START) { // Ritorno dalla prima
                    oggetto.y = y_EXIT; // All'ultima opzione
                } else {
                    if (oggetto.y == y_EXIT){ // Avendo y diverse, nel caso di exit
                        oggetto.y = y_SETTINGS; // Passa direttamente alla scritta SETTINGS
                    } else if (oggetto.y > 0) { // Altrimenti aggiorna normalmente
                        oggetto.y -= 1; // Aggiornamento ordinata dell'indicatore
                    }
                }
                break;

            case 66: // Se il tasto premuto è: Freccia verso il basso
                if (oggetto.y == y_EXIT) { // Ritorno dall'ultima
                    oggetto.y = y_START; // Alla prima opzione
                } else {
                    if (oggetto.y == y_SETTINGS){ // Avendo y diverse, da settings va in basso
                        oggetto.y = y_EXIT; // Passa direttamente a EXIT
                    } else if (oggetto.y < maxy - 1) { // Altrimenti aggiorna normalmente
                        oggetto.y += 1; // Aggiornamento ordinata dell'indicatore
                    }
                }
                break;

            case 10: // Se il tasto premuto è: Invio
                if (oggetto.y == y_EXIT) { // Scelta menù = EXIT
                    endwin(); // Termina la finestra
                    exit(0); // Termina l'esecuzione del programma
                }
                if (oggetto.y == y_START) { // Scelta menù = Inizia una partita
                    screen(w1,numnemici,rimbalzi,colore); // Funzione area di gioco
                    wclear(w1); // Pulizia dello schermo
                }
                if (oggetto.y == y_SETTINGS) { // Scelta menù = Impostazioni di gioco
                    settings = 1; // Avvia la possibilità di aprire le opzioni
                    oggetto.y = y_NUMNEMICI; // Viene aggiornata l'ordinata del puntatore schermo
                    
                    /* Menù delle opzioni */
                    while (settings) {
                        wclear(w1); // Pulizia dello schermo
                        box(w1, ACS_VLINE, ACS_HLINE); // Crea una box all'interno della finestra di gioco
                        wbkgd(w1, COLOR_PAIR(GRE_BL)); // Sfondo: VERDE / NERO
                        
                        wattron(w1, COLOR_PAIR(WHITE_BLACK)); // Start: BIANCO / NERO
                        mvwprintw(w1, oggetto.y, oggetto.x - 2, "-->"); // Stampa del puntatore del menù
                        wattroff(w1, COLOR_PAIR(WHITE_BLACK)); // End: BIANCO / NERO
                        
                        wattron(w1, COLOR_PAIR(RED_BL)); // Start: ROSSO / NERO
                        mvwprintw(w1, y_NUMNEMICI-3, xbase - 20 , "Attenzione: Le modifiche vengono salvate automaticamente.");
                        wattroff(w1, COLOR_PAIR(RED_BL)); // End: ROSSO / NERO
                        
                        /* Riga opzione - NUMERO DEI NEMICI */
                        mvwprintw(w1, y_NUMNEMICI, xbase, "Selezionare numero nemici:");
                        wattron(w1, COLOR_PAIR(WHITE_BLACK)); // Start: BIANCO / NERO
                        mvwprintw(w1, y_NUMNEMICI, xbase + 30, "%d",numnemici);
                        wattroff(w1, COLOR_PAIR(WHITE_BLACK)); // End: BIANCO / NERO
                        
                        /* Riga opzione - COLORE DELLA NAVE */
                        mvwprintw(w1, y_COLORE, xbase, "Selezionare colore nave:");
                        if(colore == 0) { // Colore BIANCO
                            wattron(w1, COLOR_PAIR(WHITE_BLACK)); // Start: BIANCO / NERO
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", bianco); // Stampa stringa colore BIANCO
                            printnavetta(maxx/5,3*maxy/5,w1); // Stampa navetta per esempio visivo
                            wattroff(w1, COLOR_PAIR(WHITE_BLACK)); // End: BIANCO / NERO
                        }
                        if(colore == 1) { // Colore CIANO
                            wattron(w1, COLOR_PAIR(CY_BL)); // Start: CIANO / NERO
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", ciano); // Stampa stringa colore CIANO
                            printnavetta(maxx/5,3*maxy/5,w1); // Stampa navetta per esempio visivo
                            wattroff(w1, COLOR_PAIR(CY_BL)); // End: CIANO / NERO
                        }
                        if(colore == 2) { // Colore ROSSO
                            wattron(w1, COLOR_PAIR(RED_BL)); // Start: ROSSO / NERO
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", rosso); // Stampa stringa colore ROSSO
                            printnavetta(maxx/5,3*maxy/5,w1); // Stampa navetta per esempio visivo
                            wattroff(w1, COLOR_PAIR(RED_BL)); // End: ROSSO / NERO
                        }
                        if(colore == 3) { // Colore VERDE
                            wattron(w1, COLOR_PAIR(GRE_BL)); // Start: VERDE / NERO
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", verde); // Stampa stringa colore VERDE
                            printnavetta(maxx/5,3*maxy/5,w1); // Stampa navetta per esempio visivo
                            wattroff(w1, COLOR_PAIR(GRE_BL));  // End: VERDE / NERO
                        }
                        if(colore == 4) { // Colore GIALLO
                            wattron(w1, COLOR_PAIR(YEL_BL)); // Start: GIALLO / NERO
                            mvwprintw(w1, y_COLORE, xbase + 30, "%s", giallo); // Stampa stringa colore GIALLO
                            printnavetta(maxx/5,3*maxy/5,w1); // Stampa navetta per esempio visivo
                            wattroff(w1, COLOR_PAIR(YEL_BL));  // End: GIALLO / NERO
                        }
                        
                        /* Riga opzione - RIMBALZI TRA NEMICI */
                        mvwprintw(w1, y_RIMBALZI, xbase, "Rimbalzi tra nemici:");
                        if(rimbalzi == 1) { // Rimbalzi attivati
                            wattron(w1, COLOR_PAIR(GRE_BL)); // Start: VERDE / NERO
                            mvwprintw(w1, y_RIMBALZI, xbase + 30, "%s", rimbalzi_ON); // Stampa stringa rimbalzi ON
                            wattroff(w1, COLOR_PAIR(GRE_BL)); // End: VERDE / NERO
                            
                            wattron(w1, COLOR_PAIR(WHITE_BLACK)); // Start: BIANCO / NERO
                            for(i=1;i<numnemici+1;i++){ // Stampa della quantità di nemici selezionati
                                printnemicolv1_f1(6*i ,maxy/5,w1); // Stampa nemici
                            }
                            wattroff(w1, COLOR_PAIR(WHITE_BLACK)); // End: BIANCO / NERO
                        }
                        if(rimbalzi == 0) { // Rimbalzi disattivati
                            wattron(w1, COLOR_PAIR(RED_BL)); // Start: ROSSO / NERO
                            mvwprintw(w1, y_RIMBALZI, xbase + 30, "%s", rimbalzi_OFF); // Stampa stringa rimbalzi OFF
                            wattroff(w1, COLOR_PAIR(RED_BL)); // End: ROSSO / NERO
                            
                            wattron(w1, COLOR_PAIR(WHITE_BLACK));  // Start: BIANCO / NERO
                            for(i=1;i<numnemici+1;i++){ // Stampa della quantità di nemici selezionati
                                printnemicolv1_f2(6*i ,maxy/5,w1); // Stampa nemici
                            }
                            wattroff(w1, COLOR_PAIR(WHITE_BLACK));  // End: BIANCO / NERO
                        }
                        
                        /* Riga opzione - EXIT */
                        mvwprintw(w1, y_EXITSETTINGS, xbase + 10, "EXIT");

                        /* Opzione tasti di movimento e selezione */
                        key = wgetch(w1); // Assegna il carattere digitato alla variabile
                        switch (key) {
                            case 66: // Se il tasto premuto è: Freccia verso il basso
                                if (oggetto.y == y_EXITSETTINGS) { // Ritorna dall'ultima
                                    oggetto.y = y_NUMNEMICI; // Alla prima opzione
                                    oggetto.x = xbase - 2; // E riprende la x della prima opzione
                                } else {
                                    if (oggetto.y == y_RIMBALZI) { // Se passa da rimbalzi, il puntatore:
                                        oggetto.y = y_EXITSETTINGS; // Prende le ordinate di EXIT
                                        oggetto.x = xbase + 8; // Prende le ascisse di EXIT
                                    } else {
                                        oggetto.y += 2; // Aggiornamento ordinata dell'indicatore
                                    }
                                }
                                break;
                            case 65: // Se il tasto premuto è: Freccia verso l'alto
                                if (oggetto.y == y_NUMNEMICI) { // Ritorna dalla prima
                                    oggetto.y = y_EXITSETTINGS; // All'ultima opzione
                                    oggetto.x = xbase + 8; // E riprende la x della opzione EXIT
                                } else {
                                    if (oggetto.y == y_EXITSETTINGS) { // Se passa da EXIT, il puntatore:
                                        oggetto.y = y_RIMBALZI; // Prende le ordinate di RIMBALZI
                                        oggetto.x = xbase - 2; // Prende le ascisse di RIMBALZI
                                    } else {
                                        oggetto.y -= 2;  // Aggiornamento ordinata dell'indicatore
                                    }
                                }
                                break;
                            case 67: // Se il tasto premuto è: Freccia verso destra
                            
                                /* Opzione NUMERO NEMICI */
                                if (oggetto.y == y_NUMNEMICI) { // Se il puntatore si trova a NUMERO NEMICI
                                    if(numnemici != ENEM_TEST) { // Se non ha raggiunto il massimo di nemici spawnabili
                                        numnemici++; // Aumenta il numero di nemici
                                    } else {
                                        numnemici = 0; // Se ha superato il massimo dei nemici, torna a zero
                                    }
                                }
                                
                                /* Opzione COLORE NAVICELLA */
                                if (oggetto.y == y_COLORE) { // Se il puntatore si trova a COLORE NAVICELLA
                                    if(colore != 4) { // Se non ha raggiunto l'ultimo colore selezionabile (GIALLO)
                                        colore++; // Aumenta l'ID del colore
                                    } else {
                                        colore = 0; // Se ha superato il massimo ID colore, torna a zero (BIANCO)
                                    }
                                }
                                
                                /* Opzione RIMBALZI */
                                if (oggetto.y == y_RIMBALZI) { // Se il puntatore si trova a RIMBALZI
                                    if (rimbalzi == 0) { // Se sono disattivati
                                        rimbalzi = 1; // Li attiva
                                    } else {
                                        rimbalzi = 0; // Altrimenti se sono attivi, li disattiva
                                    }
                                }
                                break;
                                
                            case 68: // Se il tasto premuto è: Freccia verso sinistra

                                /* Opzione NUMERO NEMICI */
                                if (oggetto.y == y_NUMNEMICI) { // Se il puntatore si trova a NUMERO NEMICI
                                    if(numnemici != 0) {  // Se non ha raggiunto il minimo di nemici spawnabili
                                        numnemici--; // Diminusci il numero di nemici
                                    } else {
                                        numnemici = ENEM_TEST; // Se ha superato il minimo dei nemici, torna al massimo
                                    }
                                }

                                /* Opzione COLORE NAVICELLA */
                                if (oggetto.y == y_COLORE) { // Se il puntatore si trova a COLORE NAVICELLA
                                    if (colore != 0) { // Se non ha raggiunto il primo colore selezionabile (BIANCO)
                                        colore--; // Diminuisce l'ID del colore
                                    } else {
                                        colore = 4; // Se ha superato il minimo ID colore, torna al massimo (GIALLO)
                                    }
                                }

                                /* Opzione RIMBALZI */
                                if (oggetto.y == y_RIMBALZI) { // Se il puntatore si trova a RIMBALZI
                                    if (rimbalzi == 1) {  // Se sono attivati
                                        rimbalzi = 0; // Li disattiva
                                    } else {
                                        rimbalzi = 1;  // Altrimenti se sono disattivati, li attiva
                                    }
                                }
                                
                                break;
                                
                            case 10: // Se il tasto premuto è: Invio
                                if (oggetto.y == y_EXITSETTINGS) { // Se il cursore si trova su EXIT
                                    settings = 0; // Disattiva la modalità delle impostazioni, torna al menù principale
                                }
                                break;
                        }
                        xbase = (maxx / 2) - 10 + fixscreen; // Reset delle ascisse di default
                    }
                    /* Reset valori per il ritorno al menù */
                    oggetto.x = (maxx / 2) - 12 + fixscreen; // Reset delle ordinate del puntatore
                    oggetto.y = y_SETTINGS; // Reset delle ascisse del puntatore
                }
                break;
        }
        wclear(w1);  // Pulizia dello schermo
    }
}
