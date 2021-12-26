#include "./process.h"

/**
 * Generatore coordinate del proiettile principale
 *
 * int x = Ascissa del proiettile.
 * int y = Ordinata del proiettile.
 * int direzione = Direzione del proiettile.
 * int* pipe,reciv = Pipes. */
void proiettile(int x, int y, int direzione, int *pipe) {
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    int diagonale = 0;
    int tmp = 0;
    Bullet proiettile;
    if (direzione == 0) proiettile.id = 0 ;
    if (direzione == 1) proiettile.id = 1 ;
    proiettile.ready=0;
    do {
        ++x;
        if ( (x % DIAGONALE == 1) && (direzione == 0) ) {
            ++diagonale;
        }
        if ( (x % DIAGONALE == 1) && (direzione == 1) ) {
            --diagonale;
        }
        proiettile.x = x;
        proiettile.y = y + diagonale;

        close(pipe[0]);
        write(pipe[1], &proiettile, sizeof(Bullet));
        usleep(20);
    } while ( (x <= maxx) || (((y+diagonale) <= maxy) && ((y+diagonale) >= 3)) );
    
    usleep(20);
    proiettile.x=-1;
    proiettile.y=-1;
    close(pipe[0]);
    write(pipe[1], &proiettile, sizeof(Bullet));
}

/**
 * Generatore coordinate della nave principale
 *
 * Player player = Struttura del giocatore principale.
 * int* pipe = Pipe.
 * int sys_slownes = Velocità di gioco. */
void gestore_input(int *pipe, int sys_slownes) {
    int c;
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    Player player;
    player.coordinata.x = 5;
    player.coordinata.y = 10;
    player.proiettile.x = 0;
    player.proiettile.y = 0;
    player.proiettile.ready = 0;
    player.id = getpid();
    player.angolo = 0;

    timeout(0);

    while (true) {
        //meglio usare la getch, cosi non fa refresh allo schermo principale
        c = getch();
        // ottenimento dati da keyboard
        if (c != ERR) {
            switch (c) {
                //aggiungendo gli altri due casi e possibile muoversi anche nell'asse x
                case KEY_UP:
                    if (player.coordinata.y > LARGHEZZA + 1) {
                        player.coordinata.y--;
                    }
                    break;
                case KEY_DOWN:
                    if (player.coordinata.y < maxy - LARGHEZZA) {
                        player.coordinata.y++;
                    }
                    break;
                    // lancio proiettile
                case (int) ' ':
                    player.proiettile.ready = PRONTO;
                default:
                    break;
            }
            while (c == getch()) { ; }
        }
        write(pipe[1], &player, sizeof(Player));
        player.proiettile.ready = UCCISA;
        napms(sys_slownes);
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
int enemyLV1_old(int x,int y,int id,int direzione,int *sender,int *receiver) {
    srand(time(NULL) * getpid());
    close(sender[0]);
    close(receiver[1]);
    //Inizializzazione navicella nemica
    int decremento = 0;
    int skipframe = 10;
    int alive = 3; //Stato navicella nemica

    //Inizializzazione nemico,info del nemico
    Player nemico;
    nemico.proiettile.x = 0;
    nemico.proiettile.y = 0;
    nemico.proiettile.ready = 0;
    nemico.coordinata.x = x;
    nemico.coordinata.y = y;
    nemico.id = id;
    //Risoluzione e pipe per inviare le coordinate
    int maxy, maxx;
    int send_info[7] = {};
    int rec[ENEM_TEST + 1] = {};
    getmaxyx(stdscr, maxy, maxx);

    //Ciclo che gesitisce il nemico
    while (nemico.coordinata.x > 0 && alive) {
        //Ciclo che gestisce il rimbalzo
        while (nemico.coordinata.y >= 4 && nemico.coordinata.y <= maxy - 3) {

            //invio dati
            nemico.proiettile.id = alive;
            nemico.angolo = direzione;

            //sincronizzazione + scambio di info
            write(sender[1], &nemico,sizeof(Player));
            napms(1);
            ++decremento;
            if (decremento == skipframe) {
                decremento = 0;
            }
            read(receiver[0], rec, (ENEM_TEST + 1) * sizeof(int));
            //impostazione skip dei frame
            
            if (rec[0] == 0) {
                alive = 0;
            }
            if (rec[id + 1] == -1) {
                --alive;
            }
            if (alive == 0) {
                nemico.coordinata.y = -1;
            }
            // dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
            if (rec[id + 1] == 1) {
                direzione = !direzione;
                rec[id + 1] = 0;
            }
            // Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
            if (nemico.proiettile.x <= -1 && (rand() % 1250 == 1)) {
                nemico.proiettile.x = nemico.coordinata.x;
                nemico.proiettile.y = nemico.coordinata.y;
            }
            //avanzamento poiettile
            if (decremento % 2 == 1) {
                --nemico.proiettile.x;
            }
            //adesso aggiorniamo i dati solo una volta
            if (decremento == 0) {
                if (direzione) {
                    nemico.coordinata.y--;
                } else {
                    nemico.coordinata.y++;
                }
            }
        }

        --nemico.coordinata.x; //La navicella nemica avanza finchè non arriva alla x del player
        --nemico.coordinata.x;
        --nemico.coordinata.x;
        --nemico.coordinata.x;
        --nemico.coordinata.x;
        --nemico.coordinata.x;
        direzione = !direzione; //Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (nemico.coordinata.y <= 3) {
            nemico.coordinata.y++;
        }
        if (nemico.coordinata.y >= maxy - 2) {
            nemico.coordinata.y--;
        }
    }

    //Nel caso in cui arrivi alla fine,per non bloccare le pipes


    close(sender[1]);
    close(receiver[0]);
    exit(0);
}

/**
 * Area di gioco dove verranno gestite stampa e collisioni
 *
 * WINDOW* w1 : Finestra di stampa. */
void screen(WINDOW *w1) {

    wbkgd(w1, COLOR_PAIR(WHITE_BLACK)); //Inizializzazione schermo nero con caratteri bianchi
    int life = 3; //Vite del giocatore
    Player player;

    /* Inizializzazione pipes e processi */
    int tmp[2];
    int bullet_p[2];
    int enemy_frame[2];
    int playerpipe[2];
    pipe(tmp);
    pipe(enemy_frame);
    pipe(bullet_p);
    pipe(playerpipe);
    pid_t nemici, spawn, giocatore, proiett, secondo;
    Bullet proiettili[2] = {};

    /* Strutture per la gestione dei nemici */
    int pr_rec[MAX_PROIETTILI][3] = {};
    int jump[ENEM_TEST + 1] = {1};
    int kill_pr[ENEM_TEST] = {};
    int helper = 0;
    int maxenemies = ENEM_TEST;
    int arrint[ENEM_TEST][7] = {}; //Contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    Player arr[ENEM_TEST] = {}; //Contiene le info di tutti in nemici, controllare enemyLV1 per piu info
    Bullet proiettil;

    /* Flag, contatori e distanze */
    int i, coordinata, decremento, w = 0, k = 0, delta, direzione, flag; //Contatori
    int y_spawner; //Indica in che ordinata andrà a spawnare il nemico
    int hit; // Flag se la navetta principale è stata colpita
    int player_started = 1; // Game-Start
    int invincibility = 0; // Flag e durata di invincibilità
    int killed = 0; //Indica i nemici uccisi
    int jumpbox = 6; // (>2) Distanza di rimbalzo
    int hitbox = 2; //Distanza dei caratteri dal centro
    int num_proiettili = 0;
    int maxy, maxx;
    int flag_pr[2] = {};
    getmaxyx(stdscr, maxy, maxx);
    /*
    for (i = 0; i < ENEM_TEST; i++) {
        pipe(tmp[i]);
        pipe(enemy_frame[i]);
    }
    */

    giocatore = fork();
    switch (giocatore) {
        case 0: // 1) Processo navicella principale
            gestore_input( playerpipe, 30);
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
                                if(decremento % 2 == 0) direzione = !direzione;
                                y_spawner = coordinata % (maxy - 2); //Si prende il modulo per scegliere la coordinata dello sprite
                                decremento = (decremento * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse x)
                                enemyLV1_old(maxx - decremento - 3, y_spawner, k, 0, tmp, enemy_frame);
                                //enemyLV1_new(70, coordinata * 3 * 2, k, 0, tmp, enemy_frame);
                                exit(-1);
                                break;
                            default: // 4) Processo bombe nemiche
                                k++;
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
                        close(tmp[i][1]);
                        close(enemy_frame[i][0]);
                    }*/
                    close(tmp[1]);
                    close(enemy_frame[0]);
                    // finche non raggiungo il gameover,scrivo lo schermo
                    while (player_started) {
                        close(playerpipe[1]);
                        read(playerpipe[0], &player, sizeof(Player));

                        if (player.proiettile.ready == PRONTO && num_proiettili == 0) {
                            flag_pr[0] = 0;
                            flag_pr[1] = 0;
                            ++num_proiettili;
                            ++num_proiettili;
                            proiett = fork();
                            switch (proiett) {
                                case 0:
                                    close(bullet_p[0]);
                                    proiettile(player.coordinata.x, player.coordinata.y, 0, bullet_p);
                                    exit(0);
                                    break;
                                default:
                                    secondo = fork();
                                    switch (secondo) {
                                        case 0:
                                            close(bullet_p[0]);
                                            proiettile(player.coordinata.x, player.coordinata.y, 1, bullet_p);
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
                            //if (arr[i].coordinata.y > -1) {
                                
                                read(tmp[0], &arr[i],sizeof(Player));
                                napms(1);
                            //}
                        }

                        // Lettura pipe del proiettile
                        for(i=0;i<num_proiettili;i++) {
                            //close(bullet_p[1]);
                            read(bullet_p[0], &proiettil, sizeof(Bullet));
                            if (proiettil.id == 0) proiettili[0] = proiettil;
                            if (proiettil.id == 1) proiettili[1] = proiettil;
                        }

                        werase(w1);

                        // CONTROLLO COLLISIONI

                        for (i = 0; i < ENEM_TEST; i++) {
                            //if (arr[i].coordinata.y > -1) {
                                ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                                for (w = i + 1; w < ENEM_TEST; w++) {
                                    // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                                    // attenzione a non ridurla troppo altrimenti ci potrbbero essere conflitti di sprite

                                    // controllo distanza tra i due nemici                  controllo se sono nella stessa x e che abbiano direzione diversa
                                    if (kill_pr[i] == 0 && kill_pr[w] == 0) {
                                        if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) < jumpbox) &&
                                            arr[i].coordinata.x == arr[w].coordinata.x && arr[i].angolo !=
                                                                                          arr[w].angolo) {   // ulteriore controllo di direzione,attenzione quando si modifica qui

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
                                            }
                                        }
                                    }
                                }
                                //collisione navetta/limite con nemico
                                if ((arr[i].coordinata.x <= 6) || ( arr[i].coordinata.x == player.coordinata.x) ||
                                    life == 0) {
                                    //questo killa definitivamente tutti i nemici,ma serve principalmente in caso di gameover
                                    //jump e usata per inviare info ai processi,in questo caso li killa tutti
                                    jump[0] = 0;
                                    player_started = 0;
                                }

                                //stampa nemici
                                stampanemici(w1,arr[i]);

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
                                            invincibility = 60;
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
                                        (proiettili[w].x - arr[i].coordinata.x < hitbox && proiettili[w].x - arr[i].coordinata.x >= 0) &&
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
                            //}
                        }

                        // STAMPE

                        // DEBUG -- Stampe debug proiettile
                        mvwprintw(w1,10,10,"x: %d",proiettili[0].x);
                        mvwprintw(w1,11,10,"y: %d",proiettili[1].x);
                        mvwprintw(w1,12,10,"proiettili: %d",num_proiettili);

                        // Stampa navetta

                        invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                        print_info(player.proiettile.ready, life, w1, maxx);

                        // Stampa proiettili
                        for (i = 0; i < num_proiettili; i++) {

                            if ( (proiettili[1].x == -1 && proiettili[1].y == -1) &&
                                 (proiettili[0].x == -1 && proiettili[0].y == -1) &&
                                 (num_proiettili != 0) ) {
                                --num_proiettili;
                                --num_proiettili;
                            }

                            if (proiettili[1].y >= 2 && flag_pr[1] == 0) { //In modo da non collidere con la linea separatrice
                                mvwaddch(w1, proiettili[1].y, proiettili[1].x, '=');
                            }
                            if(flag_pr[0] == 0){
                                mvwaddch(w1, proiettili[0].y, proiettili[0].x, '=');
                            }
                        }

                        //sincronizzazione processi + invio info su rimbalzi/uccisioni
                        for (i = 0; i < maxenemies; i++) {
                            // invio normale
                            //if (arr[i].coordinata.y > -1)
                            //{
                                write(enemy_frame[1], jump, (ENEM_TEST + 1) * sizeof(int));
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

                        //riduzione cicli necessari generali,migliorabile(maxenemies = maxenemies - killed;)
                        while (killed) {
                            --killed;
                            --maxenemies;
                        }
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
    //chiusura lettore input
    kill(player.id, SIGKILL);
}




/* int enemyLV1_new(int x, int y, int id, int direzione, int *sender, int *receiver) {
    int a;
    int b;

    //Inizializzazione navicella nemica
    pid_t enemy;
    int p_enemy[2];
    pipe(p_enemy);
    int alive = VIVA; //Stato navicella nemica

    //Inizializzazione nemico,info del nemico
    Player nemico;
    nemico.proiettile.x = 0;
    nemico.proiettile.y = 0;
    nemico.proiettile.ready = 0;
    nemico.coordinata.x = x;
    nemico.coordinata.y = y;
    nemico.angolo = direzione;
    nemico.id = id;

    //Risoluzione e pipe per inviare le coordinate
    int maxy, maxx;
    int send_info[7] = {};
    int rec[ENEM_TEST + 1] = {};
    close(receiver[1]);
    getmaxyx(stdscr, maxy, maxx);

    enemy = fork();
    switch (enemy) {

        case 0:
            //Ciclo che gesitisce il nemico
            while (nemico.coordinata.x > 0 && alive) {
                //Ciclo che gestisce il rimbalzo
                while (nemico.coordinata.y >= 0 && nemico.coordinata.y <= maxy) {
                    //Creazione processo: (P)schermo - (C)navicella/proiettile
                    //eliminato processo inutile del proiettile
                    //possibile eliminazione secondaria dato che anche qui non si fa altro che decrementare
                    --nemico.proiettile.x;
                    close(p_enemy[0]);
                    if (direzione) {
                        nemico.coordinata.y--;
                    } else {
                        nemico.coordinata.y++;
                    }
                    if (write(p_enemy[1], &nemico, sizeof(Player)) <= 0) {
                        mvprintw(maxy / 2, maxx / 2, "Error writing enemy");
                        refresh();
                    }
                    if (nemico.coordinata.y == maxy || nemico.coordinata.y == 0){
                        --nemico.coordinata.x; //La navicella nemica avanza finchè non arriva alla x del player
                        --nemico.coordinata.x;
                        --nemico.coordinata.x;
                        direzione = !direzione; //Cambio direzione navicella nemica (per il rimbalzo)
                    }
                }
            }
            break;
            //In caso di errore della fork()
        case -1:
            refresh();
            exit(-1);
            break;

            //Comunicazione delle info tramite pipe
        default:
            //Ciclo che gesitisce il nemico
            do{
                close(p_enemy[1]);
                if (read(p_enemy[0], &nemico, sizeof(Player)) <= 0) {
                    mvprintw(maxy / 2, maxx / 2, "Error reading enemy");
                    refresh();
                }
                close(sender[0]);
                close(receiver[1]);

                write(sender[1], &nemico, sizeof(Player));
                read(receiver[0], rec, (ENEM_TEST + 1) * sizeof(int));
                //la read serve per mettere il processo in waiting per il prossimo frame,altrimenti puo esserci un
                //processo veloce che invia info piu velocemente rispetto ad altri

                //killa navicella nemica
                if (rec[0] == 0 || rec[id + 1] == -1) {
                    nemico.coordinata.y = -1;
                    alive = UCCISA;
                }
                //dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
                if (rec[id + 1]) {
                    direzione = !direzione;
                    rec[id + 1] = 0;
                }
                //Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
                if (nemico.proiettile.x <= -1 && (rand() % 2 == 0)) {
                    nemico.proiettile.x = nemico.coordinata.x;
                    nemico.proiettile.y = nemico.coordinata.y;
                }
            }
            while (alive);
            break;
    }

    //Incrementi delle coordinate.y per rientrare nel ciclo
    if (nemico.coordinata.y <= 0) {
        nemico.coordinata.y++;
    } else {
        nemico.coordinata.y--;
    }

    //Nel caso in cui arrivi alla fine,per non bloccare le pipes

    close(p_enemy[0]);
    close(p_enemy[1]);
    close(sender[1]);
    exit(0);
}

// stampa info player(non necessaria)
void printpl_info(WINDOW *w1, int *arr) {
    int i = 0;
    mvwprintw(w1, 0, 15, "%d,%d,%d,%d,%d,%d,%d ", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6]);
}

void clear_matrix(Player arr[ENEM_TEST]) {
    int i = 0;
    while (i < ENEM_TEST) {
        arr[i].coordinata.x = 1000;
        arr[i].coordinata.y = 1000;
        ++i;
    }
}
*/
