#include "./process.h"

/**
 * Generatore coordinate del proiettile principale
 *
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa del proiettile.
 * int y = Ordinata del proiettile.
 * int direzione = Direzione del proiettile.
 * int* pipe = Pipe per la comunicazione. */
void proiettile(WINDOW* w,int x, int y, int direzione, int *pipe) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(w, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Struttura del proiettile */
    Bullet proiettile; // Inizializzazione struttura del proiettile
    proiettile.id = direzione; // Il proiettile verso l'alto e quello verso il basso avranno ID diversi
    proiettile.ready = SCARICO; // Il proiettile è stato sparato
    proiettile.x = x; // Il proiettile prende le ascisse della navicella principale
    proiettile.y = y; // Il proiettile prende le ordinate della navicella principale
    int diagonale = 0; // Diagonale effettuata dal proiettile (ordinate)

    /* Movimento del proiettile */
    close(pipe[LETTURA]); // Chiusura della pipe in lettura
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
        proiettile.y = y + diagonale; // Viene assegnato il proiettile il valore della navicella + la diagonale.
        usleep(DELAY_1);
        write(pipe[SCRITTURA], &proiettile, sizeof(Bullet)); // Si comunica la nuova posizione del proiettile
        usleep(DELAY_2); // Delay per la sincronizzazione tra processi
    } while ( (proiettile.x <= maxx-2) || ( (proiettile.y <= maxy-2) && (proiettile.y >= LINEA_STACCO) ) );
    /* Il proiettile avanza finché non raggiunge la fine dello schermo */

    /* Termine esecuzione proiettile */
    proiettile.x = OUT_OF_RANGE; // L'ascissa del proiettile viene impostata fuori dallo schermo
    proiettile.y = OUT_OF_RANGE; // L'ordinata del proiettile viene impostata fuori dallo schermo
    write(pipe[SCRITTURA], &proiettile, sizeof(Bullet)); // Si comunica la nuova posizione del proiettile
    usleep(DELAY_2); // Delay per la sincronizzazione tra processi
    close(pipe[SCRITTURA]); // Chiusura di sicurezza per la pipe in scrittura
}

/**
 * Generatore coordinate della bomba nemica
 *
 * WINDOW* w = Finestra di gioco.
 * int x = Ascissa della bomba.
 * int y = Ordinata della bomba.
 * int id = Identificativo della bomba.
 * int* pipe = Pipe per la comunicazione. */
void bomba(WINDOW* w,int x, int y, int id,int *pipe) {

    /* Struttura della bomba */
    Bullet bomba; // Inizializzazione della bomba
    bomba.y = y; // La bomba prende le y in entrata (della navicella nemica)
    bomba.x = x; // La bomba prende le x in entrata (della navicella nemica)
    bomba.riconoscimento = id; // Si associa l'id della navicella nemica con quello del proiettile
    bomba.ready = PRONTO; // La bomba è pronta per essere sparata
    int skipframe = 0; // Variabile per rallentare il movimento della bomba

    /* Movimento della bomba */
    close(pipe[LETTURA]); //Chiusura lato lettura della pipe
    do {
        // Avanza verso il giocatore principale
        if(skipframe % 2 == 1){ // Per valori dispari, la bomba avanza, per valori pari invece, rimane ferma
            bomba.x--; // La bomba avanza verso il giocatore/schermo sinistro
        }else{
            skipframe = 0;
        }
        usleep(DELAY_1);
        write(pipe[SCRITTURA], &bomba, sizeof(Bullet)); // Scrittura della struttura sulla pipe
        usleep(DELAY_2);// Delay per la sincronizzazione tra processi
        //napms(ENEM_TEST); // Ritardo per rallentare la bomba nemica
        ++skipframe; // Si incrementa la variabile per rallentare la bomba
    } while (bomba.x >= 0); //La bomba avanza finchè non raggiunge il bordo sinistro dello schermo

    /* Termine esecuzione bomba */
    bomba.x = OUT_OF_RANGE; // La bomba nemica ha ora una x fuori dallo schermo
    bomba.ready = BORDO; // Si segnala allo schermo che la bomba ha raggiunto il bordo
    write(pipe[SCRITTURA], &bomba, sizeof(Bullet)); // Scrittura della struttura sulla pipe
    usleep(DELAY_2);// Si crea un delay per la sincronizzazione
    close(pipe[SCRITTURA]); // Chiusura di sicurezza per la pipe in scrittura
}

/**
 * Generatore coordinate della nave principale
 *
 * int* pipe = Pipe.
 * int sys_slownes = Velocità di gioco. */
void nave(int *pipe, int sys_slownes) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione

    /* Struttura del player */
    Player player; // Inizializzazione del player
    player.coordinata.x = X_START_NAVE; // Il player inizia da un'ascissa arbitraria
    player.coordinata.y = Y_START_NAVE; // Il player inizia da un'ordinata arbitraria
    player.proiettile.x = SCARICO; // Il proiettile non è ancora stato sparato
    player.proiettile.y = SCARICO; // Il proiettile non è ancora stato sparato
    player.proiettile.ready = SCARICO; // Il proiettile non è ancora stato sparato
    player.id = getpid(); // Si prende il pid del player
    int c; // Carattere letto da input tastiera ( Convertito in intero )

    timeout(0); //Per non bloccare l'esecuzione del programma dato che si aspetta un input da tastiera

    /* Movimento del player */
    close(pipe[LETTURA]); // Chiusura lato lettura della pipe
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
        write(pipe[SCRITTURA], &player, sizeof(Player)); // Si scrive la struttura su un buffer
        player.proiettile.ready = SCARICO; // Il proiettile è stato sparato, caricatore SCARICO.
        napms(10); // Delay per la sincronizzazione dei processi
    }
}

/**
 * Spawner dei nemici di livello 1
 *
 * int x = Ascissa del nemico.
 * int y = Ordinata del nemico.
 * int id = Identificativo del nemico (per organizzazione).
 * int direzione = Direzione di partenza del nemico.
 * int* sender,receiver = Pipes. */
void nemico(int x,int y,int id,int direzione,int *sender,int *receiver) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione
    srand(time(NULL) * getpid()); // Inizializzazione del motore generatore di numeri casuali

    /* Struttura del nemico */
    Player nemico; // Inizializzazione del nemico
    nemico.proiettile.x = OUT_OF_RANGE; // Inizializzazione del proiettile nemico (ascissa)
    nemico.proiettile.y = OUT_OF_RANGE; // Inizializzazione del proiettile nemico (ordinata)
    nemico.proiettile.ready = SCARICO; // Inizializzazione del proiettile nemico (caricatore)
    nemico.proiettile.riconoscimento = id; // Inizializzazione del proiettile nemico (ID)
    nemico.coordinata.x = x; // Il nemico inizia da un'ascissa assegnata
    nemico.coordinata.y = y; // Il nemico inizia da un'ordinata assegnata
    nemico.id = id; // Il nemico ottiene l'id assegnatogli
    int decremento = 0; // Variabile per rallentare il movimento del nemico
    int skipframe = SKIP_FRAME_DELAY; // Variabile per rallentare il movimento del nemico
    int vite = 3; // Vite della navicella nemica
    int rec[ENEM_TEST + 1] = {}; // Vettore di raccolta informazioni su rimbalzi tra nemici e morti

    /* Movimento del nemico */
    close(sender[LETTURA]); // Chiusura lato lettura della pipe che spedisce
    close(receiver[SCRITTURA]); // Chiusura lato scrittura della pipe che riceve
    while (vite) { // Si continua a ciclare finchè le vite del nemico sono uguali a 0.
        while (nemico.coordinata.y >= LINEA_STACCO + 1 && nemico.coordinata.y <= maxy - 3) { // Ciclo del rimbalzo dei nemici

            nemico.proiettile.id = vite;
            nemico.angolo = direzione; // Viene scritta la direzione del nemico all'interno della struttura
            write(sender[SCRITTURA], &nemico,sizeof(Player)); // Invio della struttura del nemico
            
            napms(6); // Delay per la sincronizzazione di processi
            
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
            
            read(receiver[LETTURA], rec, (ENEM_TEST + 1) * sizeof(int)); // Lettura delle info vite/rimbalzi
            usleep(20); // Delay per la sincronizzazione di processi
            
            /* Informazioni su rimbalzi e uccisioni */
            if (rec[0] == 0) {  // Codice per terminzaione speciale
                vite = UCCISA; // La navicella nemica non ha più vite
            }
            if (rec[id + 1] < 0) { // Se arriva il codice di hit
                vite += rec[id + 1]; // La navicella nemica perde una vita
            }
            if (vite == 0) { // Se la navicella ha finito le vite
                nemico.coordinata.y = OUT_OF_RANGE; // Prende una nuova coordinata per uscire dallo schermo
            }
            // Dall'array jump estrae il suo id, serve per il rimbalzo in caso di collisioni con le navette nemiche
            if (rec[id + 1] == 1) {
                direzione = !direzione; // Si cambia la direzione della navicella nemica
                rec[id + 1] = 0;
            }
            
            nemico.proiettile.ready = SCARICO; // La bomba nemica è scarica di default
            if (nemico.proiettile.x == OUT_OF_RANGE && rand() % 2 == 1) { // Se la bomba non è nello schermo e arriva segnale
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

    // Nel caso in cui arrivi alla fine, per non bloccare le pipes
    close(sender[SCRITTURA]); // Chiusura di sicurezza: lato scrittura della pipe che spedisce
    close(receiver[LETTURA]); // Chiusura di sicurezza lato lettura della pipe che riceve
    exit(0); // Una volta terminato, si termina il processo
}

/**
 * Area di gioco dove verranno gestite stampa e collisioni
 *
 * WINDOW* w1 : Finestra di stampa.
 * int num_nemici : Numero di nemici selezionati dalle impostazioni.
 * int vite : Numero di vite selezionate dalle impostazioni.
 * int colore : Colore selezionato dalle impostazioni. */
void screen(WINDOW *w1, int num_nemici, int vite, int colore) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione
    wbkgd(w1, COLOR_PAIR(WHITE_BLACK)); // Inizializzazione schermo nero con caratteri bianchi
    clock_t start, stop; // Variabili per misurare il tempo
    int fps = 0; // Variabile frame per secondo
    int fps_counter = 0; // Counter per gli fps
    int total_fps = 0; // Media fps
    int seconds = 0; // secondi totali
    double res = 0; // Variabile di supporto fps

    /* Struttura per la gestione del player */
    Player player; // Inizializzazione struttura player
    int life = vite; // Vite del giocatore
    int invincibility = 0; // Flag e durata della invincibilità
    Bullet proiettil; // Struttura di appoggio per la lettura del processo proiettile
    Bullet proiettili[2] = {}; // Struttura per memorizzare i dati dei proiettili
    int flag_pr[2] = {}; // Flag per disattivazione proiettili in caso di hit dei nemici
    int num_proiettili = 0; // Numero di proiettili attualmente in gioco

    /* Strutture per la gestione dei nemici */
    int jump[ENEM_TEST + 1] = {1}; // Array per invio info su rimbalzi e uccisioni

/* Ogni nemico riceve il suo codice speciale mediante questo array
   se un elemento dell'array vale 1 il corrispondente nemico rimbalza
   se l'elemento vale 0 il nemico continua nella sua direzione
   se un elemento dell'array vale -1 o minori il corrispondente nemico rimbalza
*/
    
    
    int kill_pr[ENEM_TEST] = {0}; // Array per uccidere i processi
    int maxenemies = num_nemici; // Numero di nemici a schermo
    //int arrint[num_nemici][7]; // Contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    Player arr[ENEM_TEST] = {}; // Contiene le info di tutti i nemici, controllare funzione nemico per più info
    Bullet bombe[MAX_PROIETTILI] = {}; // Contiene le info di tutte le bombe nemiche
    int coordinata; // Variabile per spawnare i nemici all'interno dello schermo
    int decremento; // Variabile per distanziare i nemici all'interno dello schermo al momento di spawn
    int identificativo = 0; // Identificativo di ogni processo nemico
    int delta; // Differenza di distanza tra il centro e il bordo dello sprite
    int direzione; // Direzione della navicella nemica
    int killed = 0; // Indica i nemici uccisi
    int y_spawner; // Indica in che ordinata andrà a spawnare il nemico
    int num_bombe = 0; // Indica il numero di bombe attualmente in gioco

    /* Contatori e distanze */
    int i, w; //Contatori vari
    int player_started = 1; // Flag di Game-Start
    int jumpbox = (LARGHEZZA * 2) - 1; // Distanza di rimbalzo tra un nemico e un altro
    int hitbox = LARGHEZZA - 1; // Distanza dei caratteri dal centro

    /* Inizializzazione pipes e processi */
    int enemy_receiver[2]; // Pipe per il processo nemico, si occupa di ricevere la struttura
    int enemy_sender[2]; // Pipe per il processo nemico, si occupa di scrivere le posizioni per rimbalzi e uccisioni
    int bullet_p[2]; // Pipe per il processo proiettile, si occupa di ricevere la struttura
    int playerpipe[2]; // Pipe per il processo navicella principale, si occupa di ricevere la struttura
    int bomba_p[2]; // Pipe per il processo bomba nemica, si occupa di ricevere la struttura
    pipe(enemy_receiver); // Crea la pipe per il processo nemico
    pipe(enemy_sender); // Crea la pipe per il processo nemico
    pipe(bullet_p); // Crea la pipe per il processo proiettile
    pipe(playerpipe); // Crea la pipe per il processo navicella principale
    pipe(bomba_p); // Crea la pipe per il processo bomba nemica
    pid_t nemici, spawn, giocatore, proiett, secondo, bomb; // Variabile per i processi

    /* Inizio del gioco */
    giocatore = fork();
    switch (giocatore) {
        case 0: // 1) Processo della navicella principale
            nave(playerpipe, ENEM_TEST + 2);
            exit(0); // Una volta terminato, esce dall'esecuzione
            break;
        default:
            nemici = fork();
            switch (nemici) {
                case 0:
                    // Spawner di processi nemici
                    for (coordinata = 1; coordinata < maxenemies + 1; coordinata++) {
                        spawn = fork();
                        switch (spawn) {
                            case 0: // 2) Processo dei nemici
                                coordinata = (coordinata * 3 *
                                              2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse y)
                                decremento = coordinata /
                                             (maxy - 2); //Ogni volta che si supera il maxy viene decrementata la x
                                    if (decremento % 2 == 0) direzione = !direzione;
                                y_spawner = coordinata %
                                            (maxy - 2); //Si prende il modulo per scegliere la coordinata dello sprite
                                decremento = (decremento * 3 *
                                              2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse x)
                                nemico(maxx - decremento - 4, y_spawner, identificativo, direzione, enemy_receiver,
                                       enemy_sender);
                                exit(-1); // Una volta terminato, esce dall'esecuzione
                                break;
                            default: // Continua l'esecuzione del programma, incrementando l'id del nemico
                                identificativo++;
                                break;
                        }
                    }
                    exit(0); // Una volta terminato, esce dall'esecuzione
                    break;
                default: // 3) Processo Area di Gioco
                    wait((int *) 0); // Si aspetta l'avvio dei nemici prima di iniziare la scrittura schermo
                    signal(SIGCHLD,
                           SIG_IGN); // Serve per eviatare la creazione di processi zomie quando i figli terminano da soli
                    /* Chiusura pipes */
                    close(enemy_receiver[SCRITTURA]);
                    close(enemy_sender[LETTURA]);
                    close(playerpipe[SCRITTURA]);
                    while (player_started) { // Finche non si raggiunge il gameover, scrive lo schermo
                        // Contatore fps
                        ++fps;
                        start = clock();

                        // Si leggono le coordinate del giocatore
                        read(playerpipe[LETTURA], &player, sizeof(Player));

                        /*  Lancio dei proiettili navetta in caso di input */
                        if (player.proiettile.ready == PRONTO && num_proiettili == 0) {
                            beep(); // Suono
                            // Flag che segnalano i nemici
                            flag_pr[PROIETTILE_BASSO] = SCARICO;
                            flag_pr[PROIETTILE_ALTO] = SCARICO;
                            ++num_proiettili;
                            ++num_proiettili;
                            proiett = fork();
                            switch (proiett) { // 4) Processi proiettile nave principale
                                case 0:
                                    // 4.1 Lancio primo proiettile
                                    proiettile(w1, player.coordinata.x, player.coordinata.y, PROIETTILE_BASSO, bullet_p);
                                    exit(0); // Una volta terminato, esce dall'esecuzione
                                    break;
                                default:
                                    signal(SIGCHLD,
                                           SIG_IGN); // Per non creare processi Zombie in caso di autoterminazione
                                    secondo = fork();
                                    switch (secondo) {
                                        case 0:
                                            // 4.2 Lancio secondo proiettile
                                            proiettile(w1, player.coordinata.x, player.coordinata.y, PROIETTILE_ALTO, bullet_p);
                                            exit(0); // Una volta terminato, esce dall'esecuzione
                                            break;
                                        default:
                                            // Per non creare processi Zombie in caso di autoterminazione
                                            signal(SIGCHLD, SIG_IGN);
                                            break;
                                    }
                                    break;
                            }
                        }

                        // Lettura pipe dei nemici
                        for (i = 0; i < maxenemies; i++) {
                            read(enemy_receiver[LETTURA], &arr[i], sizeof(Player));
                        }

                        /* Lancio delle bombe nemiche in caso di input */
                        for (i = 0; i < num_bombe + 1; i++) {
                            // Se è possibile lanciare le bombe                      
                            if (bombe[i].ready == SCARICO && arr[i].proiettile.ready == PRONTO &&
                                num_bombe < MAX_PROIETTILI) {
                                num_bombe++; // Si aumenta il numero di bombe in gioco
                                bombe[i].ready = 1; // Bomba lanciata
                                bomb = fork(); //Creazione del processo bombe
                                switch (bomb) { // 5) Processi bombe nemiche
                                    case 0:
                                        // Richiamo della funzione per lanciare una bomba nemica
                                        bomba(w1, arr[i].coordinata.x, arr[i].coordinata.y,
                                              arr[i].proiettile.riconoscimento, bomba_p);
                                        exit(0); // Una volta terminato, esce dall'esecuzione
                                        break;
                                    default:
                                        signal(SIGCHLD, SIG_IGN); // Serve per eliminare i processi zombie
                                        //Il ramo default continua l'esecuzione dell'area di gioco
                                        break;
                                }
                            }
                        }

                        // Lettura pipe del proiettile
                        for (i = 0; i < num_proiettili; i++) {
                            read(bullet_p[LETTURA], &proiettil, sizeof(Bullet));
                            if (proiettil.id == PROIETTILE_BASSO) proiettili[PROIETTILE_BASSO] = proiettil;
                            if (proiettil.id == PROIETTILE_ALTO) proiettili[PROIETTILE_ALTO] = proiettil;
                        }

                        // Lettura pipe della bomba
                        for (i = 0; i < num_bombe; i++) {
                            read(bomba_p[LETTURA], &bombe[i], sizeof(Bullet));
                        }

                        werase(w1); // Cancellazione dello schermo


                        /* -- CONTROLLO COLLISIONI -- */

                        // Collisioni in cui sono coinvolti i nemici
                        for (i = 0; i < maxenemies; i++) {
                            // Pseudo-selectionsort per controllare il rimbalzo in caso di collisioni
                            for (w = i + 1; w < maxenemies; w++) {
                                /* Modificando Jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                                 * Si controlla: la distanza tra i due nemici,
                                 *               se sono nella stessa x e che siano abbastanza vicine
                                 * Implementato un controllo per evitare i controlli successivi sui nemici già uccisi */
                                if (kill_pr[i] == 0 && kill_pr[w] == 0) {
                                    // Si controlla se hanno la stessa x e una distanza y che potrebbe collidere nei frame dopo
                                    if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) <= jumpbox) && arr[i].coordinata.x == arr[w].coordinata.x) {
                                        // Si controlla la loro posizione e se hanno una direzione che potrebbe collidere
                                        if ((arr[i].coordinata.y < arr[w].coordinata.y && arr[i].angolo == 0 && arr[w].angolo == 1) ||
                                            (arr[i].coordinata.y > arr[w].coordinata.y && arr[i].angolo == 1 && arr[w].angolo == 0)) {
                                            // Salvataggio dei processi che necessitano di rimbalzo, usando l'id dei processi stessi
                                            jump[arr[w].id + 1] = 1; // vedere jump per ulteriori spiegazioni nella dichiarazione di jump
                                            jump[arr[i].id + 1] = 1;
                                            break;
                                        } else {
                                            /* Controllo nel caso in cui alcune sprite hanno la stessa
                                             * direzione ma collidono (a seguito di qualche salto di x mentre ci
                                             * sono già altri nemici nella stessa posizione) */
                                            if (((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) <= jumpbox / 2 + 1) && arr[i].angolo == arr[w].angolo)) {
                                                // In questo caso la collisione necessita di situazioni più specifiche e solo una dovrà rimbalzare
                                                if (arr[i].coordinata.y < arr[w].coordinata.y) {
                                                    if (arr[i].angolo) {
                                                        jump[arr[w].id + 1] = 1;
                                                    } else {
                                                        jump[arr[i].id + 1] = 1;
                                                    }
                                                } else {
                                                    if (arr[i].angolo) {
                                                        jump[arr[i].id + 1] = 1;
                                                    } else {
                                                        jump[arr[w].id + 1] = 1;
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            // Collisione navetta/limite con nemico
                            if (arr[i].coordinata.x <= 5) {
                                /* Questo ciclo uccide definitivamente tutti i nemici,
                                 * ma serve principalmente in caso di gameover.
                                 * Jump è usata per inviare info ai processi, in questo caso li uccide tutti */
                                jump[0] = 0;
                                player_started = 0;
                            }

                            // Collisione proiettile/navetta nemica
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
                                    if (jump[arr[i].id + 1] == 1) {
                                        jump[arr[i].id + 1] = 0; // impostazione a zero in caso in cui il nemico deve rimbalzare
                                    }//la priorità è dell'hit rispetto ai rimbalzi nemici,infatti ad ogni frame i rimbalzi vengono ricalcolati,gli hit
                                    --jump[arr[i].id + 1]; // Invio hit al nemico
                                    flag_pr[proiettili[w].id] = 1; // Disabilitazione proiettile
                                    if (arr[i].proiettile.id == 1) {
                                        // Questo ciclo serve per ridurre i nemici nei vari counter
                                        ++killed;
                                        kill_pr[arr[i].id] = 1;
                                    }
                                    --arr[i].proiettile.id;
                                }
                            }
                        }

                        // Collisioni nave principale con bomba
                        for (i = 0; i < num_bombe; i++) {
                            // Stampa della bomba nemica
                            wattron(w1, COLOR_PAIR(RED_BL)); // Start ROSSO/NERO
                            mvwaddch(w1, bombe[i].y, bombe[i].x, 'O');
                            wattroff(w1, COLOR_PAIR(RED_BL)); // Start ROSSO/NERO

                            // Controllo collisioni navetta - proiettile nemico
                            /* La prima condizione dell'if copre la parte sinistra e destra del cannone
                             * La seconda condizione copre il cannone della navicella principale
                             * Se un proiettile nemico colpisce una di queste 5 coordinate, viene
                             * Tolta una vita al giocatore e reso il player invincibile per un certo numero di frame*/
                            for (delta = -(LARGHEZZA-1); invincibility == 0 && delta < DIM - (LARGHEZZA-1); delta++) {
                                if ((bombe[i].y == player.coordinata.y + delta &&
                                     bombe[i].x == player.coordinata.x - (LARGHEZZA+1)) ||
                                    (bombe[i].y == player.coordinata.y &&
                                     bombe[i].x == player.coordinata.x)) {
                                    invincibility = INVINCIBILITA; // Viene impostato un valore/tempo di invincibilità
                                    --life; // Il giocatore perde una vita
                                    break;
                                }
                            }
                        }
                        // Condizione d'uscita: Game over, il giocatore ha finito le vite
                        if (life == UCCISA) {
                            jump[0] = 0; // Si terminano i processi
                            player_started = 0; // Si imposta il game-over
                        }


                        // Controllo se i proiettili della navetta sono stati disabilitati, in modo da riabilitarli
                        if (flag_pr[PROIETTILE_BASSO] == 1 && flag_pr[PROIETTILE_ALTO] == 1) {  // Clear della pipe finchè i processi non finiscono
                            // Primo proiettile
                            if (proiettili[PROIETTILE_BASSO].x != OUT_OF_RANGE && proiettili[PROIETTILE_BASSO].y != OUT_OF_RANGE) {
                                proiettil.x = 0;
                                do {
                                    read(bullet_p[LETTURA], &proiettil, sizeof(Bullet));
                                } while (proiettil.x != OUT_OF_RANGE); // Condizione d'uscita del nemico, dice che è stato terminato
                                proiettili[PROIETTILE_BASSO].x = OUT_OF_RANGE;
                                proiettili[PROIETTILE_BASSO].y = OUT_OF_RANGE;
                            }
                            // Secondo proiettile
                            if (proiettili[PROIETTILE_ALTO].x != OUT_OF_RANGE && proiettili[PROIETTILE_ALTO].y != OUT_OF_RANGE) {
                                proiettil.x = 0;
                                do {
                                    read(bullet_p[LETTURA], &proiettil, sizeof(Bullet));
                                } while (proiettil.x != OUT_OF_RANGE); // Condizione d'uscita del nemico, dice che è stato terminato
                                proiettili[PROIETTILE_ALTO].x = OUT_OF_RANGE;
                                proiettili[PROIETTILE_ALTO].y = OUT_OF_RANGE;
                            }
                        }

                        // Stampa della nave principale
                        switch (colore) { // Stampa della navetta colorata (in base ai settings)
                            case 0: // Bianco
                                wattron(w1, COLOR_PAIR(WHITE_BLACK)); // Start: BIANCO/NERO
                                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                                wattroff(w1, COLOR_PAIR(WHITE_BLACK)); // End: BIANCO/NERO
                                break;
                            case 1: // Ciano
                                wattron(w1, COLOR_PAIR(CY_BL));// Start: CIANO/NERO
                                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                                wattroff(w1, COLOR_PAIR(CY_BL));// End: CIANO/NERO
                                break;
                            case 2: // Rosso
                                wattron(w1, COLOR_PAIR(RED_BL));// Start: ROSSO/NERO
                                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                                wattroff(w1, COLOR_PAIR(RED_BL));// End: ROSSO/NERO
                                break;
                            case 3: // verde
                                wattron(w1, COLOR_PAIR(GRE_BL));// Start: VERDE/NERO
                                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                                wattroff(w1, COLOR_PAIR(GRE_BL));// End: VERDE/NERO
                                break;
                            case 4: // Giallo
                                wattron(w1, COLOR_PAIR(YEL_BL));// Start: GIALLO/NERO
                                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                                wattroff(w1, COLOR_PAIR(YEL_BL));// End: GIALLO/NERO
                                break;
                        }

                        // Stampa info e stati del player
                        print_info(num_proiettili == 0, life, w1, maxx, maxy);

                        // Stampa delle navicelle nemiche
                        for (i = 0; i < maxenemies; i++) {
                            stampanemici(w1, arr[i], fps);
                        }

                        // Flag per il numero di bombe
                        for (i = 0; i < num_bombe; i++) { // Si controlla se qualche bomba ha raggiunto il bordo
                            if (bombe[i].ready == BORDO) { // Se la bomba è arrivato al massimo
                                num_bombe--; // Si riduce il numero di proiettili
                                bombe[i].ready = SCARICO; // La navicella nemica è SCARICA
                            }
                        }

                        // Controllo se i proiettili sono disabilitati per abilitarli al lancio
                        if ((proiettili[PROIETTILE_ALTO].x == OUT_OF_RANGE && proiettili[PROIETTILE_ALTO].y == OUT_OF_RANGE) &&
                            (proiettili[PROIETTILE_BASSO].x == OUT_OF_RANGE && proiettili[PROIETTILE_BASSO].y == OUT_OF_RANGE) &&
                            (num_proiettili != 0)) {
                            // Se i proiettili sono arrivati alla fine, si decrementa il numero di proiettili nello schermo
                            --num_proiettili;
                            --num_proiettili;
                        }

                        // Stampa proiettile alto
                        if (proiettili[PROIETTILE_ALTO].y >= (LINEA_STACCO-1) && flag_pr[PROIETTILE_ALTO] == 0) {
                            //In modo da non collidere con la linea separatrice
                            mvwaddch(w1, proiettili[PROIETTILE_ALTO].y, proiettili[PROIETTILE_ALTO].x, '=');
                        }

                        // Stampa proiettile basso
                        if (proiettili[PROIETTILE_BASSO].y >= (LINEA_STACCO-1) && flag_pr[PROIETTILE_BASSO] == 0) {
                            mvwaddch(w1, proiettili[PROIETTILE_BASSO].y, proiettili[PROIETTILE_BASSO].x, '=');
                        }

                        // Sincronizzazione processi con invio delle informazioni su rimbalzi/uccisioni
                        for (i = 0; i < maxenemies; i++) {
                            write(enemy_sender[SCRITTURA], jump, (ENEM_TEST + 1) * sizeof(int));
                        }

                        // Vengono resettati i rimbalzi
                        for (i = 1; i < ENEM_TEST + 1; i++) {
                            jump[i] = 0;
                        }

                        // Vengono ridotti i nemici per diminuire i cicli
                        if (killed) {
                            maxenemies -= killed;
                            killed = UCCISA; // Si reimpostano i nemici uccisi a 0
                        }

                        // Stampa degli FPS
                        if (seconds > 1) {
                            wattron(w1, COLOR_PAIR(YEL_BL));
                            mvwprintw(w1, 0, maxx-25, "FPS:%d  Media FPS:%d", fps_counter, (int) (total_fps / seconds));
                            wattroff(w1, COLOR_PAIR(YEL_BL));
                        }
                        
                        wrefresh(w1); // Refresh dello schermo
                        
                        // Fine funzione per calcolare gli FPS
                        stop = clock();
                        res = res + (double) (stop - start);
                        if (res / CLOCKS_PER_SEC * 100 >= 1) {
                            total_fps += fps;
                            ++seconds;
                            fps_counter = fps;
                            fps = 0;
                            res = 0;
                        }
                        start = 0; // Viene resettato il timer orologio: start
                        stop = 0; // Viene resettato il timer orologio: stop

                        // Uscita in caso di vittoria
                        if (maxenemies == 0) {
                            player_started = 0; // Viene impostato il game_over
                        }
                    }
            }
            break;
    }
    kill(player.id, 1); // Manda il segnale di kill al processo nave principale

    if (maxenemies == 0) { // Se i nemici sono finiti, vittoria del giocatore
        victory(w1, player.coordinata.x, player.coordinata.y);
    } else { // Altrimenti ha perso
        game_over(w1, player.coordinata.x, player.coordinata.y);
    }
    // Pulizia processi bombe/proiettili
    while (num_bombe) {
        proiettil.ready = SCARICO;
        do {
            read(bomba_p[LETTURA], &proiettil, sizeof(Bullet));
        } while (proiettil.ready == BORDO);
        --num_bombe;
    }
    while (num_proiettili) {
        proiettil.x = SCARICO;
        do {
            read(bullet_p[LETTURA], &proiettil, sizeof(Bullet));
        } while (proiettil.x != OUT_OF_RANGE);
        --num_proiettili;
    }

    /* Chiusura di sicurezza per tutte le pipe */
    close(bomba_p[LETTURA]);
    close(bomba_p[SCRITTURA]);
    close(bullet_p[LETTURA]);
    close(bullet_p[SCRITTURA]);
    close(enemy_receiver[LETTURA]);
    close(enemy_sender[SCRITTURA]);
    close(playerpipe[LETTURA]);
    signal(SIGCHLD, SIG_DFL);
}
