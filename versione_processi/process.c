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
    proiettile.ready = 0; // Il proiettile è stato sparato
    proiettile.x = x; // Il proiettile prende le ascisse della navicella principale
    proiettile.y = y; // Il proiettile prende le ordinate della navicella principale
    int diagonale = 0; // Diagonale effettuata dal proiettile (ordinate)

    /* Movimento del proiettile */
    close(pipe[0]); // Chiusura della pipe in lettura
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
        usleep(100);
        write(pipe[1], &proiettile, sizeof(Bullet)); // Si comunica la nuova posizione del proiettile
        usleep(100); // Delay per la sincronizzazione tra processi
    } while ( (proiettile.x <= maxx-2) || ( (proiettile.y <= maxy-2) && (proiettile.y >= 3) ) );
    /* Il proiettile avanza finché non raggiunge la fine dello schermo */

    /* Termine esecuzione proiettile */
    proiettile.x=-1; // L'ascissa del proiettile viene impostata fuori dallo schermo
    proiettile.y=-1; // L'ordinata del proiettile viene impostata fuori dallo schermo
    write(pipe[1], &proiettile, sizeof(Bullet)); // Si comunica la nuova posizione del proiettile
    usleep(200); // Delay per la sincronizzazione tra processi
    close(pipe[1]); // Chiusura di sicurezza per la pipe in scrittura
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
    bomba.ready = 1; // La bomba è pronta per essere sparata
    int skipframe = 0; // Variabile per rallentare il movimento della bomba

    /* Movimento della bomba */
    close(pipe[0]); //Chiusura lato lettura della pipe
    do {
        // Avanza verso il giocatore principale
        if(skipframe % 2 == 1){ // Per valori dispari, la bomba avanza, per valori pari invece, rimane ferma
            bomba.x--; // La bomba avanza verso il giocatore/schermo sinistro
        }else{
            skipframe = 0;
        }
        usleep(100);
        write(pipe[1], &bomba, sizeof(Bullet)); // Scrittura della struttura sulla pipe
        napms(10); // Ritardo per rallentare la bomba nemica
        ++skipframe; // Si incrementa la variabile per rallentare la bomba
    } while (bomba.x >= 0); //La bomba avanza finchè non raggiunge il bordo sinistro dello schermo

    /* Termine esecuzione bomba */
    bomba.x = -1; // La bomba nemica ha ora una x fuori dallo schermo
    bomba.ready = BORDO; // Si segnala allo schermo che la bomba ha raggiunto il bordo
    write(pipe[1], &bomba, sizeof(Bullet)); // Scrittura della struttura sulla pipe
    usleep(200);// Si crea un delay per la sincronizzazione
    close(pipe[1]); // Chiusura di sicurezza per la pipe in scrittura
}

/**
 * Generatore coordinate della nave principale
 *
 * int* pipe = Pipe.
 * int sys_slownes = Velocità di gioco. */
void gestore_input(int *pipe, int sys_slownes) {

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

    timeout(0); //Per non bloccare l'esecuzione del programma dato che si aspetta un input da tastiera

    /* Movimento del player */
    close(pipe[0]); // Chiusura lato lettura della pipe
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
        write(pipe[1], &player, sizeof(Player)); // Si scrive la struttura su un buffer
        player.proiettile.ready = SCARICO; // Il proiettile è stato sparato, caricatore SCARICO.
        napms(sys_slownes); // Delay per la sincronizzazione dei processi
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
    nemico.proiettile.x = -1; // Inizializzazione del proiettile nemico (ascissa)
    nemico.proiettile.y = -1; // Inizializzazione del proiettile nemico (ordinata)
    nemico.proiettile.ready = SCARICO; // Inizializzazione del proiettile nemico (caricatore)
    nemico.proiettile.riconoscimento = id; // Inizializzazione del proiettile nemico (ID)
    nemico.coordinata.x = x; // Il nemico inizia da un'ascissa assegnata
    nemico.coordinata.y = y; // Il nemico inizia da un'ordinata assegnata
    nemico.id = id; // Il nemico ottiene l'id assegnatogli
    int decremento = 0; // Variabile per rallentare il movimento del nemico
    int skipframe = 2*ENEM_TEST; // Variabile per rallentare il movimento del nemico
    int vite = 3; // Vite della navicella nemica
    int rec[ENEM_TEST + 1] = {}; // Vettore di raccolta informazioni su rimbalzi tra nemici e morti

    /* Movimento del nemico */
    close(sender[0]); // Chiusura lato lettura della pipe che spedisce
    close(receiver[1]); // Chiusura lato scrittura della pipe che riceve
    while (vite) { // Si continua a ciclare finchè le vite del nemico sono uguali a 0.
        while (nemico.coordinata.y >= 4 && nemico.coordinata.y <= maxy - 3) { // Ciclo del rimbalzo dei nemici

            nemico.proiettile.id = vite;
            nemico.angolo = direzione; // Viene scritta la direzione del nemico all'interno della struttura
            write(sender[1], &nemico,sizeof(Player)); // Invio della struttura del nemico
            
            napms(ENEM_TEST/(ENEM_TEST / 5)+ 1); // Delay per la sincronizzazione di processi
            
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
            
            read(receiver[0], rec, (ENEM_TEST + 1) * sizeof(int)); // Lettura delle info vite/rimbalzi
            usleep(20); // Delay per la sincronizzazione di processi
            
            /* Informazioni su rimbalzi e uccisioni */
            if (rec[0] == 0) { 
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
            if (nemico.proiettile.x == -1 && rand() % 1250 == 1) { // Se la bomba non è nello schermo e arriva segnale
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

    // Nel caso in cui arrivi alla fine, per non bloccare le pipes
    close(sender[1]); // Chiusura di sicurezza: lato scrittura della pipe che spedisce
    close(receiver[0]); // Chiusura di sicurezza lato lettura della pipe che riceve
    exit(0); // Una volta terminato, si termina il processo
}

/**
 * Area di gioco dove verranno gestite stampa e collisioni
 *
 * WINDOW* w1 : Finestra di stampa. */
void screen(WINDOW *w1) {

    /* Ottenimento risoluzione della finestra */
    int maxy, maxx; // Inizializzazione variabili dello schermo
    getmaxyx(stdscr, maxy, maxx); // Funzione di ottenimento della risoluzione
    wbkgd(w1, COLOR_PAIR(WHITE_BLACK)); // Inizializzazione schermo nero con caratteri bianchi
    clock_t start,stop; // Variabili per misurare il tempo
    int fps = 0;
    int fps_counter = 0;
    double res = 0;

    /* Struttura per la gestione del player */
    Player player; // Inizializzazione struttura player
    int life = 3; ///* personalizzabile Vite del giocatore
    int hit; // Flag se la navetta principale è stata colpita
    int flag_proiettile_ready = 0; // Flag, indica se il proiettile è pronto ad essere sparato o meno
    int invincibility = 0; // Flag e durata di invincibilità
    Bullet proiettil; // Struttura di appoggio per la lettura del processo proiettile
    Bullet proiettili[2] = {}; // Struttura per memorizzare i dati dei proiettili
    int flag_pr[2] = {};
    int num_proiettili = 0; // Numero di proiettili attualmente in gioco

    /* Strutture per la gestione dei nemici */
    int pr_rec[MAX_PROIETTILI][3] = {};
    int jump[ENEM_TEST + 1] = {1};
    int kill_pr[ENEM_TEST] = {};
    int maxenemies = ENEM_TEST; ///* personalizzabile Numero di nemici in schermo
    int arrint[ENEM_TEST][7] = {}; // Contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    Player arr[ENEM_TEST] = {}; // Contiene le info di tutti in nemici, controllare enemyLV1 per piu info
    Bullet bombe[ENEM_TEST] = {};
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
    int jumpbox = 6; // Distanza di rimbalzo tra un nemico e un altro
    int hitbox = 2; // Distanza dei caratteri dal centro

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
        case 0: // 1) Processo navicella principale
            gestore_input(playerpipe, ENEM_TEST);
            exit(0);
            break;
        default:
            nemici = fork();
            switch (nemici) {
                case 0:
                    for (coordinata = 1; coordinata < maxenemies + 1; coordinata++) {
                        spawn = fork();
                        switch (spawn) {
                            case 0: // 3) Spawner di processi nemici
                                coordinata = (coordinata * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse y)
                                decremento = coordinata / (maxy - 2); //Ogni volta che si supera il maxy viene decrementata la x
                                if(decremento % 2 == 0) direzione = !direzione; ///* personalizzabile
                                y_spawner = coordinata % (maxy - 2); //Si prende il modulo per scegliere la coordinata dello sprite
                                decremento = (decremento * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse x)
                                nemico(maxx - decremento - 3, y_spawner, identificativo, direzione, enemy_receiver, enemy_sender);
                                exit(-1);
                                break;
                            default: // 4) Processo bombe nemiche
                                identificativo++;
                                break;
                        }
                    }
                    exit(0);
                    break;
                default: // 5) Processo Area di Gioco
                    //aspetto l'avvio dei nemici prima di iniziare la scrittura schermo
                    wait((int *) 0);
                    /*
                    for (i = 0; i < ENEM_TEST; i++) {
                        close(enemy_receiver[i][1]);
                        close(enemy_sender[i][0]);
                    }*/
                    close(enemy_receiver[1]);
                    close(enemy_sender[0]);
                    close(playerpipe[1]);
                    // finche non raggiungo il gameover,scrivo lo schermo
                    while (player_started) {
                        ++fps;
                        start = clock();
                        read(playerpipe[0], &player, sizeof(Player));

                        if (player.proiettile.ready == PRONTO && num_proiettili == 0) {
                            beep();
                            flag_proiettile_ready = 0;
                            flag_pr[0] = 0;
                            flag_pr[1] = 0;
                            ++num_proiettili;
                            ++num_proiettili;
                            proiett = fork();
                            switch (proiett) {
                                case 0:
                                    
                                    proiettile(w1,player.coordinata.x, player.coordinata.y, 0, bullet_p);
                                    exit(0);
                                    break;
                                default:
                                    secondo = fork();
                                    switch (secondo) {
                                        case 0:
                                            
                                            proiettile(w1,player.coordinata.x, player.coordinata.y, 1, bullet_p);
                                            exit(0);
                                            break;
                                        default:
                                            break;
                                    }
                                    break;
                            }
                        }


                        // Lettura pipe dei nemici
                        for (i = 0; i < maxenemies; i++) {
                            read(enemy_receiver[0], &arr[i],sizeof(Player));
                        }

                        // Processo bomba nemica
                        for (i = 0; i < maxenemies; i++) {
                            if (arr[i].proiettile.ready == PRONTO && bombe[arr[i].proiettile.riconoscimento].ready == SCARICO) { //Se la navicella è pronta a sparare
                                num_bombe++; //Si aumenta il numero di bombe in gioco
                                bomb = fork(); //Creazione del processo bombe
                                switch (bomb) {
                                    case 0:
                                        //Richiamo della funzione per lanciare una bomba nemica
                                        bomba(w1,arr[i].coordinata.x,arr[i].coordinata.y,arr[i].proiettile.riconoscimento,bomba_p);
                                        exit(0); //Chiusura del processo
                                        break;
                                    default:
                                        //Il ramo default continua l'esecuzione dell'area di gioco
                                        break;
                                }
                            }
                        }
                        // Lettura pipe del proiettile
                        for(i=0;i<num_proiettili;i++) {
                            //mvwprintw(w1,10+i,30,"R %d,%d",i,num_proiettili);
                            //wrefresh(w1);
                            read(bullet_p[0], &proiettil, sizeof(Bullet));
                            if (proiettil.id == 0) proiettili[0] = proiettil;
                            if (proiettil.id == 1) proiettili[1] = proiettil;
                            //mvwprintw(w1,10+i,30 + 5,"D %d,%d",i,num_proiettili);
                            //wrefresh(w1);
                        }
                        //mvwprintw(w1,10+i+1,30,"OUT_PROIETTILI %d,%d",i,num_proiettili);
                        //wrefresh(w1);

                        // Lettura pipe della bomba
                        for(i=0;i<num_bombe;i++) {
                            // Viene effettuata la read soltanto per il numero di bombe in campo
                            //mvwprintw(w1,10+i,10,"R %d,%d",i,num_bombe);
                            //wrefresh(w1);
                            read(bomba_p[0], &bombe[i], sizeof(Bullet));
                            //mvwprintw(w1,10+i,10 + 5,"D %d,%d",i,num_bombe);
                            //wrefresh(w1);
                            for(j=0;j<maxenemies;j++) {
                                // Si cerca la corrispondenza tra l'id della bomba e della navicella nemica
                                if (bombe[i].riconoscimento == arr[j].proiettile.riconoscimento) {
                                    // Una volta riconosciuta, copia la sua struttura
                                    arr[j].proiettile.x = bombe[i].x;
                                    arr[j].proiettile.y = bombe[i].y;
                                }
                            }
                        }
                        //mvwprintw(w1,10+i+1,10,"OUT_BOMBE %d,%d",i,num_bombe);
                        //wrefresh(w1);

                        

                        werase(w1);

                        // CONTROLLO COLLISIONI

                        for (i = 0; i < maxenemies; i++) {
                            //if (arr[i].coordinata.y > -1) {
                            ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                            for (w = i + 1; w < maxenemies; w++) {
                                // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                                // attenzione a non ridurla troppo altrimenti ci potrbbero essere conflitti di sprite

                                // controllo distanza tra i due nemici                  controllo se sono nella stessa x e che abbiano direzione diversa
                                if (kill_pr[i] == 0 && kill_pr[w] == 0) {
                                    if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) < jumpbox) &&
                                        arr[i].coordinata.x ==
                                        arr[w].coordinata.x) {   // ulteriore controllo di direzione,attenzione quando si modifica qui

                                        if ((arr[i].coordinata.y < arr[w].coordinata.y && arr[i].angolo == 0 &&
                                             arr[w].angolo == 1) ||
                                            (arr[i].coordinata.y > arr[w].coordinata.y && arr[i].angolo == 1 &&
                                             arr[w].angolo == 0)) {
                                            // salvataggio dei processi che necessitano di rimbalzo,usando l'id dei processi
                                            jump[arr[w].id + 1] = 1;
                                            jump[arr[i].id + 1] = 1;
                                            // info di debug
                                            // mvwprintw(w1, arr[i].id + 3, 30, "Hit");
                                            break;
                                        } else {

                                            if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) <
                                                 jumpbox / 2 + 1)) {
                                                if (arr[i].coordinata.y < arr[w].coordinata.y && arr[i].angolo &&
                                                    arr[w].angolo) {
                                                    jump[arr[i].id + 1] = 1;
                                                    break;
                                                } else {
                                                    if (arr[i].coordinata.y > arr[w].coordinata.y && !arr[i].angolo &&
                                                        !arr[w].angolo) {
                                                        jump[arr[w].id + 1] = 1;
                                                        break;
                                                    }
                                                }

                                            }

                                        }

                                    }
                                }
                            }
                            //collisione navetta/limite con nemico
                            if ((arr[i].coordinata.x <= 5) || 
                              //(arr[i].coordinata.x < player.coordinata.x) &&
                              //abs(arr[i].coordinata.y - player.coordinata.y) < 2 ||
                                life == 0) {
                                //questo killa definitivamente tutti i nemici,ma serve principalmente in caso di gameover
                                //jump e usata per inviare info ai processi,in questo caso li killa tutti
                                jump[0] = 0;
                                player_started = 0;
                            }

                            //collisione navetta-proiettile_nemico
                            //Se la navetta non è nello stato di invincibilita
                            if (invincibility == 0) {
                                /* La prima condizione dell'if copre la parte sinistra e destra del cannone
                                 * La seconda condizione copre il cannone della navicella principale
                                 * Se un proiettile nemico colpisce una di queste 5 coordinate, viene
                                 * Abilitata la flag "hit" con valore true e reso il player invincibile */
                                for (delta = -2; delta < DIM - 2; delta++) {
                                    if ((arr[i].proiettile.y == player.coordinata.y + delta &&
                                         arr[i].proiettile.x == player.coordinata.x - 4) ||
                                        (arr[i].proiettile.y == player.coordinata.y &&
                                         arr[i].proiettile.x == player.coordinata.x)) {
                                        mvwaddch(w1, player.coordinata.y, player.coordinata.x, 'X');
                                        hit = 1;
                                        invincibility = 120;
                                    }
                                }
                            }

                            /* Se la navetta principale è stata colpita, la flag hit e impostata a 1
                             * Questo fa sì che il giocatore principale perda una vita e venga resettata la flag a 0. */
                            if (hit == 1) {
                                life--;
                                hit = 0;
                            }

                            //collisione proiettile-navetta_nemica
                            for (w = 0; w < num_proiettili; ++w) {
                                if (flag_pr[proiettili[w].id] == 0 &&
                                    (proiettili[w].x - arr[i].coordinata.x < hitbox &&
                                     proiettili[w].x - arr[i].coordinata.x >= 0) &&
                                    ((hitbox > proiettili[w].y - arr[i].coordinata.y &&
                                      -hitbox < proiettili[w].y - arr[i].coordinata.y) ||
                                     (-hitbox < proiettili[w].y - arr[i].coordinata.y &&
                                      hitbox > proiettili[w].y - arr[i].coordinata.y))) {
                                    //possiamo sfruttare questo meccanismo per spawnare nemici da lv 1 a lv 2,e in generale per togliere la vita ai nemici
                                    jump[arr[i].id + 1] = -1;
                                    mvwaddch(w1, arr[i].coordinata.y, arr[i].coordinata.x, '#');

                                    flag_pr[proiettili[w].id] = 1;
                                    //serve per ridurre i nemici nei vari counter
                                    if (arr[i].proiettile.id == 1) {
                                        ++killed;
                                        kill_pr[arr[i].id] = 1;
                                    }
                                }
                            }
                        }

                        // STAMPE

                        /*
                        // DEBUG -- Stampe debug proiettile
                        mvwprintw(w1,10,10,"x: %d",proiettili[0].x);
                        mvwprintw(w1,11,10,"y: %d",proiettili[1].x);
                        mvwprintw(w1,12,10,"proiettili: %d",num_proiettili);
                         */

                        if (flag_pr[0] == 1 && flag_pr[1] == 1)
                        {   //clear della pipe finche i processi non finiscono
                           if(proiettili[0].x != (-1) && proiettili[0].y != -1)
                           {
                            proiettil.x = 0;
                            do{
                                read(bullet_p[0], &proiettil, sizeof(Bullet));
                            }while (proiettil.x != -1);
                            proiettili[0].x = -1;
                            proiettili[0].y = -1;                            
                            }
                            if(proiettili[1].x != (-1) && proiettili[1].y != -1)
                            {
                            proiettil.x = 0;
                            do{
                                read(bullet_p[0], &proiettil, sizeof(Bullet));
                            }while (proiettil.x != -1);
                            proiettili[1].x = -1;
                            proiettili[1].y = -1;
                            }
                        }
                        // Se il proiettile della navetta è stato resettato o non è ancora stato lanciato:
                        if (((proiettili[0].x == (-1) && proiettili[0].y == (-1))
                          && (proiettili[1].x == (-1) && proiettili[1].y == (-1))) ||
                           ( (proiettili[0].x == (0) && proiettili[0].y == (0))
                          && (proiettili[1].x == (0) && proiettili[1].y == (0))) )
                            flag_proiettile_ready=1;


                        wattron(w1,COLOR_PAIR(CY_BL));
                        invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                        wattroff(w1,COLOR_PAIR(CY_BL));
                        print_info(flag_proiettile_ready, life, w1, maxx, maxy);
                        
                        for(i=0;i<maxenemies;i++){
                            // Stampa delle navicelle nemiche
                            stampanemici(w1, arr[i]);
                        }

                        for(i=0;i<maxenemies;i++){ //Si controlla se qualche bomba ha raggiunto il bordo
                            if(bombe[i].ready == BORDO) { //Se il proiettile è arrivato al massimo
                                num_bombe--; // Si riduce il numero di proiettili
                                //questo non può funzionare,ogni volta che si fa la read questo dato si cancella
                                bombe[i].ready = SCARICO; // La navicella nemica è SCARICA
                            }
                        }

                        //mvwprintw(w1,10,10,"num:%d",num_bombe);


                        // Check se i proiettili sono ancora attivi
                        if ( (proiettili[1].x == -1 && proiettili[1].y == -1) &&
                             (proiettili[0].x == -1 && proiettili[0].y == -1) &&
                             (num_proiettili != 0) ) {
                            --num_proiettili; // riduzione numero dei proiettili
                            --num_proiettili;
                        }
                        // Stampa proiettili
                        // Per evitare stampe dopo la linea separatrice
                        if (proiettili[1].y >= 2 && flag_pr[1] == 0) { // Controllo se il proiettile ha già colpito il nemico
                            mvwaddch(w1, proiettili[1].y, proiettili[1].x, '=');
                        }

                        if(flag_pr[0] == 0){ // Controllo se il proiettile ha già colpito il nemico
                            mvwaddch(w1, proiettili[0].y, proiettili[0].x, '=');
                            }
                        }


                        for (i = 0; i < maxenemies; i++) {
                            // invio normale
                            //if (arr[i].coordinata.y > -1)
                            //{
                                write(enemy_sender[1], jump, (ENEM_TEST + 1) * sizeof(int));
                               // if (kill_pr[arr[i].id] == 1)
                               // {
                               //     arr[i].coordinata.y = -5;
                               // }
                            //}
                        }

                        //reset dei rimbalzi,necessario
                        for (i = 1; i < ENEM_TEST + 1; i++) {
                            jump[i] = 0;
                        }
                        
                        maxenemies -= killed;
                        killed = 0;

                        
                        stop = clock();
                        res = res + (double)(stop - start);
                        if(res/CLOCKS_PER_SEC*100 >= 1){
                            fps_counter = fps;
                            fps = 0;
                            res = 0;
                        }
                        start = 0;
                        stop = 0;
                        mvwprintw(w1,0,60,"FPS:%d",fps_counter);
                        wrefresh(w1);

                        if (maxenemies == 0) {
                            player_started = 0;
                        }
                    }
            }
            break;
    }

    if (maxenemies == 0) {
        victory(w1, player.coordinata.x, player.coordinata.y);
    } else {
        game_over(w1, player.coordinata.x, player.coordinata.y);
    }
    //pulizia processi
    while(num_bombe){
        proiettil.ready = 0;
        do{
            read(bomba_p[0], &proiettil, sizeof(Bullet));
        } while (proiettil.ready == BORDO);
        --num_bombe; 
    }
    while(num_proiettili){
        proiettil.x = 0;
        do{
            read(bullet_p[0], &proiettil, sizeof(Bullet));
        } while (proiettil.x != -1);
        --num_proiettili; 
    }
    //chiusura lettore input
    kill(player.id, SIGKILL);
    close(bomba_p[0]);
    close(bomba_p[1]);
    close(bullet_p[0]);
    close(bullet_p[1]);
    close(enemy_receiver[0]);
    close(enemy_sender[1]);
    close(playerpipe[0]);    
}
