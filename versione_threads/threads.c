#include "./threads.h"

typedef struct{
    int x;
    int y;
    int direzione;
    WINDOW* w;
} parametro_proiettile;
/**
 * Generatore coordinate del proiettile principale
 *
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa del proiettile.
 * int y = Ordinata del proiettile.
 * int direzione = Direzione del proiettile. */
void* proiettile(void* p_proiettile) {

    parametro_proiettile* parametro;
    parametro = (parametro_proiettile*) p_proiettile;

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(parametro->w, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Struttura del proiettile */
    Bullet proiettile; // Inizializzazione struttura del proiettile
    proiettile.id = parametro->direzione; // Il proiettile verso l'alto e quello verso il basso avranno ID diversi
    proiettile.ready = 0; // Il proiettile è stato sparato
    proiettile.x = parametro->x; // Il proiettile prende le ascisse della navicella principale
    proiettile.y = parametro->y; // Il proiettile prende le ordinate della navicella principale
    int diagonale = 0; // Diagonale effettuata dal proiettile (ordinate)

    /* Movimento del proiettile */
    do {
        /* Avanzamento lungo l'asse delle ascisse */
        proiettile.x++; // Il proiettile avanza lungo le ascisse
        
        /* Avanzamento lungo l'asse delle ordinate */
        if ( (proiettile.x % DIAGONALE == 1) && (proiettile.id == PROIETTILE_BASSO) ) {
            ++diagonale; // La diagonale del proiettile viene incrementata ogni "DIAGONALE" x.
        }
        if ( (proiettile.x % DIAGONALE == 1) && (proiettile.id == PROIETTILE_ALTO) ) {
            --diagonale; // La diagonale del proiettile viene decrementata ogni "DIAGONALE" x.
        }
        proiettile.y = parametro->y + diagonale; // Viene assegnato il proiettile il valore della navicella + la diagonale.
        usleep(200);
    } while ( (proiettile.x <= maxx-2) || ( (proiettile.y <= maxy-2) && (proiettile.y >= 3) ) );
    /* Il proiettile avanza finché non raggiunge la fine dello schermo */

    /* Termine esecuzione proiettile */
    proiettile.x=-1; // L'ascissa del proiettile viene impostata fuori dallo schermo
    proiettile.y=-1; // L'ordinata del proiettile viene impostata fuori dallo schermo
}

typedef struct{
    int x;
    int y;
    int id;
    WINDOW* w;
} parametro_bomba;
/**
 * Generatore coordinate della bomba nemica
 *
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della bomba.
 * int y = Ordinata della bomba.
 * int id = Identificativo della bomba. */
void* bomba(void* p_bomba) {

    parametro_bomba* parametro;
    parametro = (parametro_bomba*) p_bomba;

    /* Struttura della bomba */
    Bullet bomba; // Inizializzazione della bomba
    bomba.y = parametro->y; // La bomba prende le y in entrata (della navicella nemica)
    bomba.x = parametro->x; // La bomba prende le x in entrata (della navicella nemica)
    bomba.riconoscimento = parametro->id; // Si associa l'id della navicella nemica con quello del proiettile
    bomba.ready = 1; // La bomba è pronta per essere sparata
    int skipframe = 0; // Variabile per rallentare il movimento della bomba

    /* Movimento della bomba */
    do {
        // Avanza verso il giocatore principale
        if(skipframe % 2 == 1){ // Per valori dispari, la bomba avanza, per valori pari invece, rimane ferma
            bomba.x--; // La bomba avanza verso il giocatore/schermo sinistro
        }else{
            skipframe = 0;
        }
        //napms(ENEM_TEST); // Ritardo per rallentare la bomba nemica
        ++skipframe; // Si incrementa la variabile per rallentare la bomba
        usleep(200);
    } while (bomba.x >= 0); //La bomba avanza finchè non raggiunge il bordo sinistro dello schermo

    /* Termine esecuzione bomba */
    bomba.x = -1; // La bomba nemica ha ora una x fuori dallo schermo
    bomba.ready = BORDO; // Si segnala allo schermo che la bomba ha raggiunto il bordo
}

/**
 * Generatore coordinate della nave principale
 *
 * int sys_slownes = Velocità di gioco. */
void* nave() {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Struttura del player */
    Player player; // Inizializzazione del player
    player.coordinata.x = 5; // Il player inizia da un'ascissa arbitraria
    player.coordinata.y = 10; // Il player inizia da un'ordinata arbitraria
    player.proiettile.x = SCARICO; // Il proiettile non è ancora stato sparato
    player.proiettile.y = SCARICO; // Il proiettile non è ancora stato sparato
    player.proiettile.ready = SCARICO; // Il proiettile non è ancora stato sparato
    player.id = getpid(); // Si prende il pid del player
    int c; // Carattere letto da input tastiera ( Convertito in intero )

    /* Movimento del player */
    while (true) {
        c = getch(); // Si aspetta un input da tastiera
        if (c != ERR) { // Nel caso l'input sia valido
            switch (c) { // Si distingue caso per caso
                case KEY_UP: // Se è stato premuto tasto in alto, la navetta va verso l'alto
                    if (player.coordinata.y > LARGHEZZA + 1) { // A patto che sia distanziata da sopra
                        player.coordinata.y--; // Decremento dell'ordinata
                    }
                    break;
                case KEY_DOWN: // Se è stato premuto tasto in basso, la navetta va verso il basso
                    if (player.coordinata.y < maxy - LARGHEZZA) { // A patto che sia distanziata da sotto
                        player.coordinata.y++; // Incremento dell'ordinata
                    }
                    break;
                    // lancio proiettile
                case (int) ' ': // Se è stato premuta la barra spaziatrice, la navetta carica un proiettile
                    player.proiettile.ready = PRONTO; // Si abilita la flag del proiettile pronto
                default:
                    break;
            }
        }
        player.proiettile.ready = SCARICO; // Il proiettile è stato sparato, caricatore SCARICO.
        napms(10);
    }
}

typedef struct{
    int x;
    int y;
    int id;
    int direzione;
} parametro_nemico;
/**
 * Spawner dei nemici di livello 1
 *
 * int x = Ascissa del nemico.
 * int y = Ordinata del nemico.
 * int id = Identificativo del nemico (per organizzazione).
 * int direzione = Direzione di partenza del nemico. */
void* nemico(void* p_nemico) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione
    srand(time(NULL)); // Inizializzazione del motore generatore di numeri casuali

    /* Struttura del nemico */
    Player nemico; // Inizializzazione del nemico
    parametro_nemico* parametro;
    parametro = (parametro_nemico*) p_nemico;
    nemico.proiettile.x = -1; // Inizializzazione del proiettile nemico (ascissa)
    nemico.proiettile.y = -1; // Inizializzazione del proiettile nemico (ordinata)
    nemico.proiettile.ready = SCARICO; // Inizializzazione del proiettile nemico (caricatore)
    nemico.proiettile.riconoscimento = parametro->id; // Inizializzazione del proiettile nemico (ID)
    nemico.coordinata.x = parametro->x; // Il nemico inizia da un'ascissa assegnata
    nemico.coordinata.y = parametro->y; // Il nemico inizia da un'ordinata assegnata
    nemico.id = parametro->id; // Il nemico ottiene l'id assegnatogli
    int id = parametro->id;
    int decremento = 0; // Variabile per rallentare il movimento del nemico
    int skipframe = 2*10; // Variabile per rallentare il movimento del nemico
    int vite = 3; // Vite della navicella nemica
    int rec[ENEM_TEST + 1] = {}; // Vettore di raccolta informazioni su rimbalzi tra nemici e morti
    int direzione = parametro->direzione;

    /* Movimento del nemico */
    while (vite) { // Si continua a ciclare finchè le vite del nemico sono uguali a 0.
        while (nemico.coordinata.y >= 4 && nemico.coordinata.y <= maxy - 3) { // Ciclo del rimbalzo dei nemici

            nemico.proiettile.id = vite;
            nemico.angolo = parametro->direzione; // Viene scritta la direzione del nemico all'interno della struttura
            
            napms(10/2); // Delay per la sincronizzazione di processi
            
            /* Algoritmo per il ritardo di gioco */
            ++decremento; // Viene incrementata la variabile per il rallentamento
            if (decremento == skipframe) { // Se questa è uguale ad un certo numero
                decremento = 0; // Si resetta il decremento
            }
            if (decremento == 0) { // Se il decremento è resettao
                if (direzione) { // Si controlla la direzione della navicella nemica
                    nemico.coordinata.y--; // Se va verso l'alto ( con direzione == 1 ) si decrementa
                } else {
                    nemico.coordinata.y++; // Se va verso il basso ( con direzione == 0 ) si incrementa
                }
            }
            
            /* Informazioni su rimbalzi e uccisioni */
            if (rec[0] == 0) {  // Codice per terminzaione speciale
                vite = 0; // La navicella nemica non ha più vite
            }
            if (rec[id + 1] == -1) {
                --vite; // La navicella nemica perde una vita
            }
            if (vite == 0) { // Se la navicella ha finito le vite
                nemico.coordinata.y = -1; // Prende una nuova coordinata per uscire dallo schermo
            }
            // dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
            if (rec[id + 1] == 1) {
                direzione = !direzione; // Si cambia la direzione della navicella nemica
                rec[id + 1] = 0;
            }
            
            nemico.proiettile.ready = SCARICO; // La bomba nemica è scarica di default
            if (nemico.proiettile.x == -1 && rand() % 2 == 1) { // Se la bomba non è nello schermo e arriva segnale
                nemico.proiettile.ready = PRONTO; // Viene sparata una nuova bomba
            }
            
        } // Fine del ciclo di rimbalzo

        ///* ottimizzabile
        nemico.coordinata.x -= 6; // La navicella nemica avanza finchè non arriva alla x del player
        direzione = !direzione; // Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (nemico.coordinata.y <= 3) { // Se il nemico ha raggiunto il bordo alto
            nemico.coordinata.y++; // Si incrementa la sua ordinata per rientrare nel ciclo
        }
        if (nemico.coordinata.y >= maxy - 2) { // Se il nemico ha raggiunto il bordo basso
            nemico.coordinata.y--; // Si decrementa la sua ordinata per rientrare nel ciclo
        }
    }
}

/**
 * Area di gioco dove verranno gestite stampa e collisioni
 *
 * WINDOW* w1 : Finestra di stampa. */
void screen_threads(WINDOW *w1, int num_nemici, int rimbalzi, int colore) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione
    wbkgd(w1, COLOR_PAIR(WHITE_BLACK)); // Inizializzazione schermo nero con caratteri bianchi
    clock_t start, stop; // Variabili per misurare il tempo
    int fps = 0;
    int fps_counter = 0;
    int total_fps = 0; // Media fps
    int seconds = 0; // secondi totali
    double res = 0;

    /* Struttura per la gestione del player */
    Player player; // Inizializzazione struttura player
    int life = 3; ///* personalizzabile Vite del giocatore
    int hit; // Flag se la navetta principale è stata colpita
    int flag_proiettile_ready = 0; // Flag, indica se il proiettile è pronto ad essere sparato o meno
    int invincibility = 0; // Flag e durata di invincibilità
    Bullet proiettil; // Struttura di appoggio per la lettura del processo proiettile
    Bullet proiettili[2] = {}; // Struttura per memorizzare i dati dei proiettili
    int flag_pr[2] = {}; // Flag per disattivazione proiettili in caso di hit dei nemici
    int num_proiettili = 0; // Numero di proiettili attualmente in gioco

    /* Strutture per la gestione dei nemici */
    int pr_rec[MAX_PROIETTILI][3];
    int jump[ENEM_TEST + 1] = {1}; // Array per invio info su rimbalzi uccisioni
    int kill_pr[ENEM_TEST] = {0};
    int maxenemies = num_nemici; ///* personalizzabile Numero di nemici in schermo
    //int arrint[num_nemici][7]; // Contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    Player arr[ENEM_TEST] = {}; // Contiene le info di tutti i nemici, controllare funzione nemico per più info
    Bullet bombe[MAX_PROIETTILI] = {}; // Contiene le info di tutte le bombe nemiche
    int coordinata; // Variabile per spawnare i nemici all'interno dello schermo
    int decremento; // Variabile per distanziare i nemici all'interno dello schermo al momento di spawn
    int identificativo = 0; // Indentificativo di ogni processo nemico
    int delta; // Differenza di distanza tra il centro e il bordo dello sprite
    int direzione; // Direzione della navicella nemica
    int killed = 0; // Indica i nemici uccisi
    int y_spawner; // Indica in che ordinata andrà a spawnare il nemico
    int num_bombe = 0; // Indica il numero di bombe attualmente in gioco

    /* Contatori e distanze */
    int i, j, w; //Contatori vari
    int player_started = 1; // Flag di Game-Start
    int jumpbox = 5; // Distanza di rimbalzo tra un nemico e un altro
    int hitbox = 2; // Distanza dei caratteri dal centro
    
    /* Threads e mutex */
    pthread_t t_nave, t_nemico, t_bomba, proiettile_alto, proiettile_basso;
    parametro_bomba* p_bomba = malloc(sizeof(parametro_bomba));
    parametro_proiettile* p_proiettile = malloc(sizeof(parametro_proiettile));
    parametro_nemico* p_nemico = malloc(sizeof(parametro_nemico));
    pthread_mutex_t mutex;
    pthread_mutex_init (&mutex, NULL);


    /* Inizio del gioco */
    pthread_create(&t_nave,NULL,nave,NULL);
    for (coordinata = 1; coordinata < maxenemies + 1; coordinata++) {
        coordinata = (coordinata * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse y)
        decremento = coordinata / (maxy - 2); //Ogni volta che si supera il maxy viene decrementata la x
        if (rimbalzi == 0) {
            if (decremento % 2 == 0) direzione = !direzione; ///* personalizzabile
        }
        y_spawner = coordinata % (maxy - 2); //Si prende il modulo per scegliere la coordinata dello sprite
        decremento = (decremento * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse x)
        p_nemico->x = maxx - decremento - 4;
        p_nemico->y = y_spawner;
        p_nemico->direzione = identificativo;
        p_nemico->id = direzione;
        pthread_create(&t_nemico,NULL,nemico,p_nemico);
    }

    // finche non raggiungo il gameover/vittoria,scrivo lo schermo
    while (player_started) {
        // Contatore fps
        ++fps;
        start = clock();

        /*  Lancio dei proiettili navetta in caso di input */
        if (player.proiettile.ready == PRONTO && num_proiettili == 0) {
            beep();
            // Flag che segnalano i nemici
            flag_pr[0] = 0;
            flag_pr[1] = 0;
            ++num_proiettili;
            ++num_proiettili;
            p_proiettile->direzione = 0;
            p_proiettile->x = player.coordinata.x;
            p_proiettile->y = player.coordinata.y;
            p_proiettile->w = w1;
            pthread_create(&proiettile_alto,NULL,proiettile,p_proiettile);

            p_proiettile->direzione = 1;
            p_proiettile->x = player.coordinata.x;
            p_proiettile->y = player.coordinata.y;
            p_proiettile->w = w1;
            pthread_create(&proiettile_basso,NULL,proiettile,p_proiettile);
        }

        // Processo bomba nemica
        for (i = 0; i < num_bombe + 1; i++) {
            // Se è possibile lanciare le bombe
            if (bombe[i].ready == SCARICO && arr[i].proiettile.ready == PRONTO && num_bombe < MAX_PROIETTILI) {
                num_bombe++; // Si aumenta il numero di bombe in gioco
                bombe[i].ready = 1; // Bomba lanciata
                // Richiamo della funzione per lanciare una bomba nemica
                p_bomba->w = w1;
                p_bomba->x = arr[i].coordinata.x;
                p_bomba->y = arr[i].coordinata.y;
                p_bomba->id = arr[i].proiettile.riconoscimento;
                pthread_create(&t_bomba,NULL,bomba,p_bomba);
            }
        }

        werase(w1);

        // CONTROLLO COLLISIONI

        for (i = 0; i < maxenemies; i++) {
            ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
            for (w = i + 1; w < maxenemies; w++) {
                // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                // attenzione a non ridurla troppo altrimenti ci potrbbero essere conflitti di sprite
                // controllo distanza tra i due nemici,controllo se sono nella stessa x e che siano abbastanza vicine
                // controllo per evitare i controlli successivi sui nemici già uccisi
                if (kill_pr[i] == 0 && kill_pr[w] == 0) {
                    // controllo se hanno la stessa x e se hanno una distanza y che potrebbe collidere nei frame dopo
                    if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) <= jumpbox) &&
                        arr[i].coordinata.x ==
                        arr[w].coordinata.x) {
                        // controllo la loro posizione e se hanno una direzione che potrebbe collidere
                        if ((arr[i].coordinata.y < arr[w].coordinata.y && arr[i].angolo == 0 &&
                             arr[w].angolo == 1) ||
                            (arr[i].coordinata.y > arr[w].coordinata.y && arr[i].angolo == 1 &&
                             arr[w].angolo == 0)) {
                            // salvataggio dei processi che necessitano di rimbalzo,usando l'id dei processi stessi
                            jump[arr[w].id + 1] = 1;
                            jump[arr[i].id + 1] = 1;
                            // info di debug
                            // mvwprintw(w1, arr[i].id + 3, 30, "Hit");
                            break;
                        } else {
                            // Controllo nel caso in cui alcune sprite hanno la stessa direzione,ma collidono (a seguito di qualche salto di x mentre ci sono già altri nemici nella stessa posizione)
                            if (((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) <= jumpbox / 2 + 1) &&
                                 arr[i].angolo == arr[w].angolo)) {
                                // in questo caso la collisione necessita di situazioni più specifiche,e solo una dovrà rimbalzare
                                if (arr[i].coordinata.y < arr[w].coordinata.y) {
                                    if (arr[i].angolo) {
                                        jump[arr[w].id + 1] = 1;
                                        //jump[arr[i].id + 1] = 0;
                                    } else {
                                        jump[arr[i].id + 1] = 1;
                                        //jump[arr[w].id + 1] = 0;
                                    }
                                } else {
                                    if (arr[i].angolo) {
                                        jump[arr[i].id + 1] = 1;
                                        //jump[arr[w].id + 1] = 0;
                                    } else {
                                        jump[arr[w].id + 1] = 1;
                                        //jump[arr[i].id + 1] = 0;
                                    }
                                }
                                break;
                            }
                        }

                    }
                }
            }
            //collisione navetta/limite con nemico
            if ((arr[i].coordinata.x <= 5)
                //(arr[i].coordinata.x < player.coordinata.x) &&
                // abilitazione delle ix in caso potesse servire
                // || abs(arr[i].coordinata.y - player.coordinata.y) < 2 && abs(arr[i].coordinata.x - player.coordinata.x) < 2
                    ) {
                //questo killa definitivamente tutti i nemici,ma serve principalmente in caso di gameover
                //jump e usata per inviare info ai processi,in questo caso li killa tutti
                jump[0] = 0;
                player_started = 0;
            }

            //collisione proiettile-navetta_nemica
            for (w = 0; w < num_proiettili; ++w) {
                // Primo controllo se il proiettile è attivo
                if (flag_pr[proiettili[w].id] == 0 &&
                    // Controlli hitbox tra proiettile navetta nemica
                    (proiettili[w].x - arr[i].coordinata.x < hitbox &&
                     proiettili[w].x - arr[i].coordinata.x >= 0) &&
                    ((hitbox > proiettili[w].y - arr[i].coordinata.y &&
                      -hitbox < proiettili[w].y - arr[i].coordinata.y) ||
                     (-hitbox < proiettili[w].y - arr[i].coordinata.y &&
                      hitbox > proiettili[w].y - arr[i].coordinata.y))) {
                    jump[arr[i].id + 1] = -1; // invio hit al nemico
                    flag_pr[proiettili[w].id] = 1; // disabilitazione proiettile
                    // serve per ridurre i nemici nei vari counter
                    if (arr[i].proiettile.id == 1) {
                        ++killed;
                        kill_pr[arr[i].id] = 1;
                    }
                }
            }
        }

        //Se la navetta non è nello stato di invincibilita
        for (i = 0; i < num_bombe; i++) {
            // stampa della bomba
            wattron(w1, COLOR_PAIR(RED_BL));
            mvwaddch(w1, bombe[i].y, bombe[i].x, 'O');
            wattroff(w1, COLOR_PAIR(RED_BL));
            // controllo collisioni navetta - proiettile nemico
            /* La prima condizione dell'if copre la parte sinistra e destra del cannone
             * La seconda condizione copre il cannone della navicella principale
             * Se un proiettile nemico colpisce una di queste 5 coordinate, viene
             * Tolta una vita al giocatore e reso il player invincibile per un certo numero di frame*/
            for (delta = -2; invincibility == 0 && delta < DIM - 2; delta++) {   // controllo hitbox
                if ((bombe[i].y == player.coordinata.y + delta &&
                     bombe[i].x == player.coordinata.x - 4) ||
                    (bombe[i].y == player.coordinata.y &&
                     bombe[i].x == player.coordinata.x)) {
                    invincibility = 180;
                    --life;
                    break;
                }
            }
        }
        // Condizione d'uscita
        if (life == 0) {
            jump[0] = 0;
            player_started = 0;
        }

        // Controllo se i proiettili della navetta sono stati disabilitati,in modo da riabilitarli
        if (flag_pr[0] == 1 && flag_pr[1] == 1) {  // Clear della pipe finchè i processi non finiscono
            // Primo proiettile
            if (proiettili[0].x != (-1) && proiettili[0].y != -1) {
                proiettil.x = 0;

                proiettili[0].x = -1;
                proiettili[0].y = -1;
            }
            // Secondo proiettile
            if (proiettili[1].x != (-1) && proiettili[1].y != -1) {
                proiettil.x = 0;

                proiettili[1].x = -1;
                proiettili[1].y = -1;
            }
        }

        switch (colore) { // stampa navetta colorata
            case 0: // Bianco
                wattron(w1, COLOR_PAIR(WHITE_BLACK));
                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                wattroff(w1, COLOR_PAIR(WHITE_BLACK));
                break;
            case 1: // Ciano
                wattron(w1, COLOR_PAIR(CY_BL));
                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                wattroff(w1, COLOR_PAIR(CY_BL));
                break;
            case 2: // Rosso
                wattron(w1, COLOR_PAIR(RED_BL));
                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                wattroff(w1, COLOR_PAIR(RED_BL));
                break;
            case 3: // verde
                wattron(w1, COLOR_PAIR(GRE_BL));
                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                wattroff(w1, COLOR_PAIR(GRE_BL));
                break;
            case 4: // Giallo
                wattron(w1, COLOR_PAIR(YEL_BL));
                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                wattroff(w1, COLOR_PAIR(YEL_BL));
                break;
            default:
                break;
        }
        print_info(num_proiettili == 0, life, w1, maxx, maxy);// stampa stato del player

        for (i = 0; i < maxenemies; i++) {
            // Stampa delle navicelle nemiche
            stampanemici(w1, arr[i], fps);
        }

        for (i = 0; i < num_bombe; i++) { //Si controlla se qualche bomba ha raggiunto il bordo
            if (bombe[i].ready == BORDO) { //Se il proiettile è arrivato al massimo
                num_bombe--; // Si riduce il numero di proiettili
                //questo non può funzionare,ogni volta che si fa la read questo dato si cancella
                bombe[i].ready = SCARICO; // La navicella nemica è SCARICA
            }
        }
        // Controllo se sono disabilitati per abilitarli al lancio
        if ((proiettili[1].x == -1 && proiettili[1].y == -1) &&
            (proiettili[0].x == -1 && proiettili[0].y == -1) &&
            (num_proiettili != 0)) {
            --num_proiettili;
            --num_proiettili;
        }
        // Stampa proiettili
        if (proiettili[1].y >= 2 && flag_pr[1] == 0) { //In modo da non collidere con la linea separatrice
            mvwaddch(w1, proiettili[1].y, proiettili[1].x, '=');
        }
        if (flag_pr[0] == 0) {
            mvwaddch(w1, proiettili[0].y, proiettili[0].x, '=');
        }
        // Sincronizzazione processi + invio info su rimbalzi/uccisioni

        //reset dei rimbalzi,necessario
        for (i = 1; i < ENEM_TEST + 1; i++) {
            jump[i] = 0;
        }

        //riduzione dei nemici
        if (killed) {
            maxenemies -= killed;
            killed = 0;
        }

        // controllo FPS
        if (seconds > 1) {
            mvwprintw(w1, 0, 60, "FPS:%d,Media FPS:%d", fps_counter, (int) (total_fps / seconds));
        }
        wrefresh(w1);
        stop = clock();
        res = res + (double) (stop - start);
        if (res / CLOCKS_PER_SEC * 100 >= 1) {
            total_fps += fps;
            ++seconds;
            fps_counter = fps;
            fps = 0;
            res = 0;
        }
        start = 0;
        stop = 0;
        // uscita in caso di vittoria
        if (maxenemies == 0) {
            player_started = 0;
        }
    }

    if (maxenemies == 0) {
        victory(w1, player.coordinata.x, player.coordinata.y);
    } else {
        game_over(w1, player.coordinata.x, player.coordinata.y);
    }
}
