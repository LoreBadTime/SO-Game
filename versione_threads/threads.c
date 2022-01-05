#include "./threads.h"

/* Ogni nemico riceve il suo codice speciale mediante questo array.
   In questa versione, l'uso di questo array è stato ridotto:
   se un elemento dell'array vale "1" il corrispondente nemico rimbalza,
   se l'elemento vale 0 il nemico continua nella sua direzione */
int jump[ENEM_TEST + 1] = {}; // Array per invio info sui rimbalzi

/* Variabili globali di supporto */
int skipframe = 10; // Numero di cicli che i nemici saltano prima di spostarsi
int end = 0; // Variabile globale per terminare tutti i threads alla fine
int p_x = 0; // Variabile x d'aiuto per il lancio dei proiettili della navetta
int p_y = 0; // Variabile y d'aiuto per il lancio dei proiettili della navetta

/* Semafori e Mutex */
sem_t proj[2]; // Semaforo lanciare i proiettili della navetta
sem_t bomb[ENEM_TEST]; // Semaforo per lanciare le bombe nemiche
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex per le zone critiche


/**
 * Generatore coordinate del proiettile principale
 *
 * void* p_proiettile = Parametro proiettile della thread function. */
void* thread_proiettile(void* p_proiettile) {

    /* Lettura del parametro */
    Bullet *realdata = NULL; // La struttura del parametro è un proiettile
    realdata = (Bullet *) p_proiettile; // Si scrive il parametro in un puntatore

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Struttura del proiettile */
    Bullet proiettile; // Inizializzazione struttura del proiettile
    proiettile.id = realdata->id; // Ottenimento id del proiettile

    int y = 0; // Il proiettile prende le ordinate della navicella principale

    /* Movimento del proiettile */
    while (end) { // Finché il thread non termina
        sem_wait(&proj[realdata->id]); // Attesa lancio proiettile da schermo
        proiettile.x = p_x; // Ottenimento delle coordinate x della navetta
        y = p_y; // Ottenimento delle coordinate y della navetta
        do {
            /* Avanzamento lungo l'asse delle ascisse */
            proiettile.x++;
            /* Avanzamento lungo l'asse delle ordinate */
            if ((proiettile.x % DIAGONALE == 1) && (proiettile.id == PROIETTILE_BASSO)) {
                ++y; // La diagonale del proiettile viene incrementata ogni "DIAGONALE" x.
            }
            if ((proiettile.x % DIAGONALE == 1) && (proiettile.id == PROIETTILE_ALTO)) {
                --y; // La diagonale del proiettile viene decrementata ogni "DIAGONALE" x.
            }
            /* Scrittura dati nello schermo/dato originale */
            realdata->y = y; // Viene ripassato al parametro la nuova ordinata
            realdata->x = proiettile.x; // Viene ripassato al parametro la nuova ascissa
            napms(5); // Delay per rallentare i proiettili
        } while (realdata->x > 0 && ((proiettile.x <= maxx - 2) || ((proiettile.y <= maxy - 2) && (proiettile.y >= LINEA_STACCO))));
        /* Il proiettile avanza finché non raggiunge la fine dello schermo */

        /* Termine esecuzione proiettile */
        realdata->x = OUT_OF_RANGE; // L'ascissa del proiettile viene impostata fuori dallo schermo
        realdata->y = OUT_OF_RANGE; // L'ordinata del proiettile viene impostata fuori dallo schermo
    }
    pthread_exit(0);
}

/**
 * Generatore coordinate della bomba nemica
 *
 * void* p_bomba = Parametro bomba della thread function. */
void* thread_bomba(void* p_bomba) {

    /* Lettura del parametro */
    Bullet *realdata = NULL; // La struttura del parametro è una bomba
    realdata = (Bullet *) p_bomba;  // Si scrive il parametro in un puntatore

    /* Struttura di supporto della bomba */
    Bullet bomba;  // Inizializzazione struttura della bomba
    bomba.id = realdata->id;  // Ottenimento id della bomba
    int skip = 0; // Variabile per rallentare il movimento della bomba

    /* Movimento della bomba */
    while (end) { // Finché il thread non termina
        sem_wait(&bomb[bomba.id]); // Attesa lancio proiettile da schermo
        bomba.x = realdata->x; // Si copiano i dati del parametro nella struttura d'appoggio
        while (bomba.x >= 0) { // La bomba avanza finchè non raggiunge il bordo sinistro dello schermo
            bomba.x--; // La bomba avanza verso il giocatore/schermo sinistro
            ++skip; // Si incrementa la variabile per rallentare la bomba
            realdata->x = bomba.x; // Viene copiato il dato sulla struttura della bomba principale
            napms(25); // Velocità della bomba
        }

        /* Termine esecuzione bomba */
        realdata->x = OUT_OF_RANGE; // La bomba nemica ha ora una x fuori dallo schermo
        realdata->ready = BORDO; // Si segnala allo schermo che la bomba ha raggiunto il bordo
    }
    pthread_exit(0);
}

/**
 * Generatore coordinate della nave principale
 *
 * void* parametro = Parametro nave della thread function. */
void* thread_nave(void *parametro) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Struttura del player */
    Player *player = NULL; // La struttura del parametro è un giocatore
    player = (Player *) parametro; // Si scrive il parametro in un puntatore
    player->coordinata.x = X_START_NAVE; // Il player inizia da un'ascissa arbitraria
    player->coordinata.y = Y_START_NAVE; // Il player inizia da un'ordinata arbitraria
    player->proiettile.x = SCARICO; // Il proiettile non è ancora stato sparato
    player->proiettile.y = SCARICO; // Il proiettile non è ancora stato sparato
    player->proiettile.ready = SCARICO; // Il proiettile non è ancora stato sparato
    int c; // Carattere letto da input tastiera ( Convertito in intero )

    timeout(CPU_NAP);// Timeout per non far andare la CPU al 100%

    /* Movimento del player */
    while (end) { // Finché il thread non termina
        c = getch(); // Si aspetta un input da tastiera
        if (c != ERR) { // Nel caso l'input sia valido
            pthread_mutex_lock(&mutex); // Blocco del mutex per proteggere sezione critica
            switch (c) { // Si distingue caso per caso
                case KEY_UP: // Se è stato premuto tasto in alto, la navetta va verso l'alto
                    if (player->coordinata.y > LARGHEZZA + 1) { // A patto che sia distanziata da sopra
                        player->coordinata.y--; // Decremento dell'ordinata
                    }
                    break;
                case KEY_DOWN: // Se è stato premuto tasto in basso, la navetta va verso il basso
                    if (player->coordinata.y < maxy - LARGHEZZA) { // A patto che sia distanziata da sotto
                        player->coordinata.y++; // Incremento dell'ordinata
                    }
                    break;
                    // Lancio proiettile
                case (int) ' ': // Se è stato premuta la barra spaziatrice, la navetta carica un proiettile
                    player->proiettile.ready = PRONTO; // Si abilita la flag del proiettile pronto
                default:
                    break;
            }
            pthread_mutex_unlock(&mutex); // Sblocco del mutex
        }
    }
    pthread_exit(0);
}

/**
 * Spawner dei nemici di livello 1
 *
 * void* p_nemico = Parametro nemico della thread function. */
void* thread_nemico(void* p_nemico) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Lettura del parametro */
    Player *enemy = NULL; // La struttura del parametro è un nemico
    enemy = (Player *) p_nemico;  // Si scrive il parametro in un puntatore
    enemy->proiettile.x = OUT_OF_RANGE; // Inizializzazione del proiettile nemico (ascissa)
    enemy->proiettile.y = OUT_OF_RANGE; // Inizializzazione del proiettile nemico (ordinata)
    enemy->proiettile.ready = SCARICO; // Inizializzazione del proiettile nemico (caricatore)
    int id = enemy->id; // Variabile di supporto dove si mette l'id del nemico
    int vite = 3; // Vite della navicella nemica
    enemy->proiettile.id = vite; // Vengono salvate le vite in un campo nemico
    int direzione = enemy->angolo; // Inizializzazione direzione della navicella
    int decremento = 0; // Variabile per rallentare il movimento del nemico

    /* Struttura di supporto della bomba */
    Player nemico; // Struttura nemico di supporto
    nemico.proiettile.x = OUT_OF_RANGE; // Inizializzazione del proiettile nemico (ascissa)
    nemico.proiettile.y = OUT_OF_RANGE; // Inizializzazione del proiettile nemico (ordinata)
    nemico.proiettile.ready = SCARICO; // Inizializzazione del proiettile nemico (caricatore)
    nemico.proiettile.riconoscimento = enemy->id; // Inizializzazione del proiettile nemico (ID)
    nemico.coordinata.x = enemy->coordinata.x; // Il nemico inizia da un'ascissa assegnata
    nemico.coordinata.y = enemy->coordinata.y; // Il nemico inizia da un'ordinata assegnata
    int *rec = NULL; // Array di supporto per i salti
    rec = jump; //?


    /* Movimento del nemico */
    while (enemy->proiettile.id) { // Si continua a ciclare finchè le vite del nemico sono uguali a 0.
        while (nemico.coordinata.y >= (LINEA_STACCO + 1) && nemico.coordinata.y <= maxy - LARGHEZZA &&
               enemy->proiettile.id) { // Ciclo del rimbalzo dei nemici

            /* Copiatura dati da struttura di supporto a puntatore */
            enemy->angolo = direzione; // Viene scritta la direzione del nemico all'interno della struttura
            enemy->coordinata.x = nemico.coordinata.x; // Il nemico inizia da un'ascissa assegnata
            enemy->coordinata.y = nemico.coordinata.y; // Il nemico inizia da un'ordinata assegnata
            enemy->proiettile.ready = nemico.proiettile.ready; // Si controlla se la bomba è pronta ad essere sparata
            rec[id + 1] = 0;
            napms(20);

            /* Algoritmo per il ritardo di gioco */
            ++decremento; // Viene incrementata la variabile per il rallentamento
            if (decremento == skipframe) { // Se questa è uguale ad un certo numero
                decremento = 0; // Si resetta il decremento
            }
            if (decremento == 0) { // Se il decremento è resettato
                if (direzione) { // Si controlla la direzione della navicella nemica
                    nemico.coordinata.y--; // Se va verso l'alto ( con direzione == 1 ) si decrementa
                } else {
                    nemico.coordinata.y++; // Se va verso il basso ( con direzione == 0 ) si incrementa
                }
            }
            
            /* Informazioni su rimbalzi e uccisioni */
            if (rec[0] == 0) {  // Codice per terminazione speciale
                enemy->proiettile.id = 0; // La navicella nemica non ha più vite
            }
            if (rec[id + 1] == 1) {
                direzione = !direzione; // Si cambia la direzione della navicella nemica
                rec[id + 1] = 0;
            }
            if (enemy->proiettile.id == 0) { // Se la navicella ha finito le vite
                nemico.coordinata.y = OUT_OF_RANGE; // Prende una nuova coordinata per uscire dallo schermo
            }
            nemico.proiettile.ready = SCARICO; // La bomba nemica è scarica di default
            if (nemico.proiettile.x == OUT_OF_RANGE && rand() % 100 == 1) { // Se la bomba non è nello schermo e arriva segnale
                nemico.proiettile.ready = PRONTO; // Viene sparata una nuova bomba
            }
        } // Fine del ciclo di rimbalzo

        nemico.coordinata.x -= (LARGHEZZA * 2); // La navicella nemica avanza finchè non arriva alla x del player
        direzione = !direzione; // Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (nemico.coordinata.y <= LINEA_STACCO) { // Se il nemico ha raggiunto il bordo alto
            nemico.coordinata.y++; // Si incrementa la sua ordinata per rientrare nel ciclo
        }
        if (nemico.coordinata.y >= maxy - 2) { // Se il nemico ha raggiunto il bordo basso
            nemico.coordinata.y--; // Si decrementa la sua ordinata per rientrare nel ciclo
        }
    }
    enemy->coordinata.y = OUT_OF_RANGE;  // L'ordinata del nemico viene impostata fuori dallo schermo
    enemy->coordinata.x = OUT_OF_RANGE;  // L'ascissa del nemico viene impostata fuori dallo schermo
    pthread_exit(0);
}

/**
 * Area di gioco dove verranno gestite stampa e collisioni
 *
 * WINDOW* w1 : Finestra di stampa. */
void screen_threads(WINDOW *w1, int num_nemici, int vite, int colore) {
    werase(w1);
    end = 1;
    jump[0] = 1;
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
    int life = vite; ///* personalizzabile Vite del giocatore
    int hit; // Flag se la navetta principale è stata colpita
    int flag_proiettile_ready = 0; // Flag, indica se il proiettile è pronto ad essere sparato o meno
    int invincibility = 0; // Flag e durata di invincibilità
    Bullet proiettil; // Struttura di appoggio per la lettura del processo proiettile
    Bullet proiettili[2] = {}; // Struttura per memorizzare i dati dei proiettili
    int flag_pr[2] = {}; // Flag per disattivazione proiettili in caso di hit dei nemici
    int num_proiettili = 0; // Numero di proiettili attualmente in gioco

    /* Strutture per la gestione dei nemici */
    int pr_rec[MAX_PROIETTILI][3];

    int kill_pr[ENEM_TEST] = {0};
    int maxenemies = num_nemici; ///* personalizzabile Numero di nemici in schermo
    //int arrint[num_nemici][7]; // Contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    Player arr[ENEM_TEST] = {}; // Contiene le info di tutti i nemici, controllare funzione nemico per più info
    Bullet bombe[ENEM_TEST] = {}; // Contiene le info di tutte le bombe nemiche
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
    pthread_t t_nave, t_nemico[ENEM_TEST], t_bomba, proiettile_alto, proiettile_basso, t_bombe[ENEM_TEST];

    /* Inizio del gioco */
    sem_init(&proj[PROIETTILE_BASSO], 0, 0);
    sem_init(&proj[PROIETTILE_ALTO], 0, 0);

    for (i = 0; i < num_nemici; i++)
        sem_init(&bomb[i], 0, 0);

    proiettili[PROIETTILE_ALTO].id = PROIETTILE_ALTO;
    proiettili[PROIETTILE_BASSO].id = PROIETTILE_BASSO;
    pthread_create(&proiettile_basso, NULL, thread_proiettile, (void *) &proiettili[PROIETTILE_BASSO]);
    pthread_create(&proiettile_alto, NULL, thread_proiettile, (void *) &proiettili[PROIETTILE_ALTO]);


    for (i = 0; i < maxenemies; i++) {
        bombe[i].ready = SCARICO;
        bombe[i].id = i;// Bomba lanciata
        pthread_create(&t_bombe[i], NULL, thread_bomba,
                       (void *) &bombe[i]); // Disabilita le collisioni per qualche motivo
    }

    pthread_create(&t_nave, NULL, thread_nave, (void *) &player);

    i = 0;
    for (coordinata = 1; i < maxenemies; coordinata++, i++) {
        mvwprintw(w1, 11 + i, 2, "lancio %d", coordinata);
        wrefresh(w1);
        coordinata = (i * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse y)
        decremento = coordinata / (maxy - 2); //Ogni volta che si supera il maxy viene decrementata la x
        if (decremento % 2 == 0) direzione = !direzione; ///* personalizzabile
        y_spawner = coordinata % (maxy - 2); //Si prende il modulo per scegliere la coordinata dello sprite
        decremento = (decremento * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse x)
        arr[i].coordinata.x = maxx - 4 - decremento;
        arr[i].coordinata.y = y_spawner;
        arr[i].id = i;
        arr[i].angolo = direzione;
        jump[i + 1] = 0;
        mvwprintw(w1, 11 + i, 10, "lanciato %d,%d,%d,%d,nemici:%d", i, arr[i].coordinata.x, arr[i].coordinata.y,
                  arr[i].id, maxenemies);
        wrefresh(w1);
        pthread_create(&t_nemico[i], NULL, thread_nemico, (void *) &arr[i]);
    }

    while (player_started) {

        usleep(CPU_NAP);//serve solo per non far andare la CPU al 100%(htop),si può rimuovere/commentare;

        // Contatore fps
        ++fps;
        start = clock();

        //  Lancio dei proiettili navetta in caso di input
        if (player.proiettile.ready == PRONTO && num_proiettili == 0) {
            pthread_mutex_lock(&mutex);
            player.proiettile.ready = SCARICO;
            beep();
            // Flag che segnalano i nemici
            flag_pr[PROIETTILE_BASSO] = 0;
            flag_pr[PROIETTILE_ALTO] = 0;
            ++num_proiettili;
            ++num_proiettili;
            p_x = player.coordinata.x;
            p_y = player.coordinata.y;
            sem_post(&proj[PROIETTILE_BASSO]);
            sem_post(&proj[PROIETTILE_ALTO]);
            pthread_mutex_unlock(&mutex);
        }

        //PUNTO IN CUI I NEMICI HANNO FINITO DI ELABORARE LE COORDINATE,SOSPENSIONE DEI NEMICI

        // Processo bomba nemica
        for (i = 0; i < num_nemici + 1; i++) {
            // Se è possibile lanciare le bombe
            if (bombe[i].ready == SCARICO && arr[i].proiettile.ready == PRONTO && num_bombe < MAX_PROIETTILI &&
                arr[i].proiettile.id > 0) {
                pthread_mutex_lock(&mutex);
                num_bombe++; // Si aumenta il numero di bombe in gioco
                bombe[i].ready = 1; // Bomba lanciata
                bombe[i].x = arr[i].coordinata.x;
                bombe[i].y = arr[i].coordinata.y;
                bombe[i].id = arr[i].proiettile.riconoscimento;
                sem_post(&bomb[i]);
                pthread_mutex_unlock(&mutex);
            }
        }

        werase(w1);
        pthread_mutex_lock(&mutex);

        // CONTROLLO COLLISIONI

        for (i = 0; i < num_nemici; i++) {
            if (arr[i].proiettile.id > 0) {
                ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                for (w = i + 1; w < num_nemici; w++) {
                    if (arr[w].proiettile.id > 0) {
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
                                    if (((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) <=
                                          jumpbox / 2 + 1) &&
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
                for (w = 0; w < MAX_PROIETTILI; ++w) {
                    // Primo controllo se il proiettile è attivo
                    if (flag_pr[proiettili[w].id] == 0 && arr[i].proiettile.id > 0 &&
                        // Controlli hitbox tra proiettile navetta nemica
                        (proiettili[w].x - arr[i].coordinata.x < hitbox &&
                         proiettili[w].x - arr[i].coordinata.x >= 0) &&
                        ((hitbox > proiettili[w].y - arr[i].coordinata.y &&
                          -hitbox < proiettili[w].y - arr[i].coordinata.y) ||
                         (-hitbox < proiettili[w].y - arr[i].coordinata.y &&
                          hitbox > proiettili[w].y - arr[i].coordinata.y))) {
                        --arr[i].proiettile.id; // invio hit al nemico
                        flag_pr[proiettili[w].id] = 1; // disabilitazione proiettile

                        // serve per ridurre i nemici nei vari counter
                        if (arr[i].proiettile.id == 0) {
                            ++killed;
                            kill_pr[arr[i].id] = 1;
                        }
                    }
                }
            }
        }

        //Se la navetta non è nello stato di invincibilita
        for (i = 0; i < num_nemici; i++) {
            // stampa della bomba
            wattron(w1, COLOR_PAIR(RED_BL));
            mvwaddch(w1, bombe[i].y, bombe[i].x, 'O');
            wattroff(w1, COLOR_PAIR(RED_BL));
            // controllo collisioni navetta - proiettile nemico
            /* La prima condizione dell'if copre la parte sinistra e destra del cannone
             * La seconda condizione copre il cannone della navicella principale
             * Se un proiettile nemico colpisce una di queste 5 coordinate, viene
             * Tolta una vita al giocatore e reso il player invincibile per un certo numero di frame*/
            for (delta = -(LARGHEZZA - 1);
                 invincibility == 0 && delta < DIM - (LARGHEZZA - 1); delta++) {   // controllo hitbox
                if ((bombe[i].y == player.coordinata.y + delta &&
                     bombe[i].x == player.coordinata.x - (LARGHEZZA + 1)) ||
                    (bombe[i].y == player.coordinata.y &&
                     bombe[i].x == player.coordinata.x)) {
                    invincibility = INVINCIBILITA;
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
        if (flag_pr[PROIETTILE_BASSO] == 1 && flag_pr[PROIETTILE_ALTO] == 1) {
            // Primo proiettile
            proiettili[PROIETTILE_BASSO].x = OUT_OF_RANGE;
            proiettili[PROIETTILE_BASSO].y = OUT_OF_RANGE;
            // Secondo proiettile   
            proiettili[PROIETTILE_ALTO].x = OUT_OF_RANGE;
            proiettili[PROIETTILE_ALTO].y = OUT_OF_RANGE;

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
        print_info(num_proiettili == 0, life, w1, maxx, maxy); // Stampa stato del player

        for (i = 0; i < num_nemici; i++) {
            // Stampa delle navicelle nemiche
            if (arr[i].proiettile.id > 0) {
                stampanemici(w1, arr[i], fps);
            }
        }

        for (i = 0; i < num_nemici; i++) { //Si controlla se qualche bomba ha raggiunto il bordo
            if (bombe[i].ready == BORDO) { //Se il proiettile è arrivato al massimo
                num_bombe--; // Si riduce il numero di proiettili
                bombe[i].ready = SCARICO; // La navicella nemica è SCARICA
            }
        }
        // Controllo se sono disabilitati per abilitarli al lancio
        if ((proiettili[PROIETTILE_ALTO].x == OUT_OF_RANGE && proiettili[PROIETTILE_ALTO].y == OUT_OF_RANGE) &&
            (proiettili[PROIETTILE_BASSO].x == OUT_OF_RANGE && proiettili[PROIETTILE_BASSO].y == OUT_OF_RANGE) &&
            (num_proiettili != 0)) {
            --num_proiettili;
            --num_proiettili;
        }
        // Stampa proiettili
        if (proiettili[PROIETTILE_ALTO].y >= 2 &&
            flag_pr[PROIETTILE_ALTO] == 0) { //In modo da non collidere con la linea separatrice
            mvwaddch(w1, proiettili[PROIETTILE_ALTO].y, proiettili[PROIETTILE_ALTO].x, '=');
        }
        if (proiettili[PROIETTILE_BASSO].y >= 2 && flag_pr[PROIETTILE_BASSO] == 0) {
            mvwaddch(w1, proiettili[PROIETTILE_BASSO].y, proiettili[PROIETTILE_BASSO].x, '=');
        }
        if (killed) {
            maxenemies -= killed;
            killed = 0;
        }

        pthread_mutex_unlock(&mutex);

        // controllo FPS
        if (seconds > 1) {
            wattron(w1, COLOR_PAIR(YEL_BL));
            mvwprintw(w1, 0, maxx - 25, "FPS:%d  Media FPS:%d", fps_counter, (int) (total_fps / seconds));
            wattroff(w1, COLOR_PAIR(YEL_BL));
        }

        mvwprintw(w1, 0, 0, " "); // Per eliminare stampe sbagliate di proiettili
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
    end = 0;
    sem_post(&proj[PROIETTILE_BASSO]);
    sem_post(&proj[PROIETTILE_ALTO]);
    for (i = 0; i < ENEM_TEST; i++)
        sem_post(&bomb[i]);

    if (maxenemies == 0) {
        victory(w1, player.coordinata.x, player.coordinata.y);
    } else {
        game_over(w1, player.coordinata.x, player.coordinata.y);
    }
    pthread_join(proiettile_basso, (void **) 0);
    pthread_join(proiettile_alto, (void **) 0);
    pthread_join(t_nave, (void **) 0);
    for (i = 0; i < num_nemici; i++) {
        pthread_join(t_nemico[i], (void **) 0);
    }
    p_x = 0;
    p_y = 0;
}
