#include "./process.h"

void proiettile(int x,int y,int *pipe,int *reciv){
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    close(pipe[0]);
    close(reciv[1]);
    int ready = 0;
    int tmp = 0;
    int sender[3] = {};
    while (x < maxx)
    {
        ++x;
        if (x % DIAGONALE == 1)
        {
            ++ready;
        }
        sender[0] = x;
        sender[1] = y;
        sender[2] = ready;
        
        write(pipe[1],sender,3*sizeof(int));
        read(reciv[0],&tmp,sizeof(int));
    }
    sender[0] = -1;
    sender[1] = -1;
    write(pipe[1],sender,3*sizeof(int));
    // questo meccanismo permette di bloccare anche al lancio di un proiettile per volta,ma e comunque modificabile
}


int enemyLV1_old(int x,int y,int id,int direzione,int *sender,int *receiver){
    
    close(sender[0]);
    close(receiver[1]);
    //Inizializzazione navicella nemica
    int decremento = 0;
    int skipframe = 20;
    int alive = 3; //Stato navicella nemica

    //Inizializzazione nemico,info del nemico
    Navetta_Nemica nemico;
    nemico.proiettile.x=0;
    nemico.proiettile.y=0;
    nemico.proiettile.ready=0;
    nemico.navnemica.x=x;
    nemico.navnemica.y=y;
    
    //Risoluzione e pipe per inviare le coordinate
    int maxy,maxx;
    int send_info[7] = {};
    int rec[ENEM_TEST + 1] = {};
    getmaxyx(stdscr, maxy, maxx);

    //Ciclo che gesitisce il nemico
    while (nemico.navnemica.x > 0 && alive) {
        //Ciclo che gestisce il rimbalzo
        while (nemico.navnemica.y >= 4 && nemico.navnemica.y <= maxy - 3) {

            --nemico.proiettile.x;
            //adesso aggiorniamo i dati solo una volta
            if (decremento == 0){
                if (direzione)
                {
                    nemico.navnemica.y--;
                }
                else
                {
                    nemico.navnemica.y++;
                }
            }
            //invio dati,tra poco provo a ripristinare il send di struttura
            send_info[4] = alive;
            send_info[5] = id;
            send_info[6] = direzione;
            send_info[0] = nemico.navnemica.x;
            send_info[1] = nemico.navnemica.y;
            send_info[2] = nemico.proiettile.x;
            send_info[3] = nemico.proiettile.y;

            //sincronizzazione + scambio di info

            write(sender[1], send_info, 7 * sizeof(int));
            read(receiver[0], rec, (ENEM_TEST + 1) * sizeof(int));
            //impostazione skip dei frame
            ++decremento;
            if (decremento == skipframe){
                decremento = 0;
            }
            if (rec[0] == 0){
                // killa navicella nemica
            }
            alive == 0;
            if (rec[id + 1] == -1){
                --alive;
            }
            if (alive == 0){
                nemico.navnemica.y = -1;
            }
            // dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
            if (rec[id + 1]){
                direzione = !direzione;
                rec[id + 1] = 0;
            }
            // Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
            if (nemico.proiettile.x <= -1 && (rand() % 2 == 0)){
                nemico.proiettile.x = nemico.navnemica.x;
                nemico.proiettile.y = nemico.navnemica.y;
            }
        }

        --nemico.navnemica.x; //La navicella nemica avanza finchè non arriva alla x del player
        --nemico.navnemica.x;
        --nemico.navnemica.x;
        direzione = !direzione; //Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (nemico.navnemica.y <= 3) {
            nemico.navnemica.y++;
        }
        if (nemico.navnemica.y >= maxy - 2) {
            nemico.navnemica.y--;
        }
    }

    //Nel caso in cui arrivi alla fine,per non bloccare le pipes


    close(sender[1]);
    close(receiver[0]);
    exit(0);
}

// Test nemici di livello 1
// x,y = coordinate.
// id = id nemico(per organizzazione)
// sender,receiver = pipes.
// direzione indica la direzione di partenza del nemico
int enemyLV1_new(int x, int y, int id, int direzione, int *sender, int *receiver) {
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


//Player contiene tutte le info del player
//sys_slownes e la velocita di gioco
void gestore_input(Bullet player, int *pipe, int sys_slownes) {
    int c;
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    player.id = getpid();
    timeout(0);

    while (true) {
        //meglio usare la getch, cosi non fa refresh allo schermo principale
        c = getch();
        // ottenimento dati da keyboard
        if (c != ERR) {
            switch (c) {
                //aggiungendo gli altri due casi e possibile muoversi anche nell'asse x
                case KEY_UP:
                    if (player.y > LARGHEZZA + 1) {
                        player.y--;
                    }
                    break;
                case KEY_DOWN:
                    if (player.y < maxy - LARGHEZZA) {
                        player.y++;
                    }
                    break;
                    // lancio proiettile
                case (int) ' ':
                    player.ready = PRONTO;
                default:
                    break;
            }
            while (c == getch()) { ; }
        }
        write(pipe[1], &player, sizeof(Bullet));
        player.ready = UCCISA;
        napms(sys_slownes);
    }
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

// Funzione principale
// Gestisce l'output a video
// Il parametro è la finestra di riferimento
void screen(WINDOW *w1) {

    /* Inizializzazione giocatore */
    int life = 3; //Vite del giocatore
    Player player;
    player.coordinata.x = 5;
    player.coordinata.y = 10;
    player.proiettile.x = 0;
    player.proiettile.y = 0;
    player.proiettile.ready = 0;
    player.id = 0;
    player.angolo = 0;

    /* Inizializzazione proiettile */
    Bullet player_input;
    player_input.x = player.coordinata.x;
    player_input.y = player.coordinata.y;
    player_input.ready = 0;
    
    Bullet proiettili[MAX_PROIETTILI] = {};
    
    /* Inizializzazione pipes e processi */
    int tmp[ENEM_TEST][2];
    int bullet_p[2];
    int bullet_ps[2];
    int enemy_frame[ENEM_TEST][2];
    int playerpipe[2];
    pipe(bullet_p);
    pipe(playerpipe);
    pipe(bullet_ps);
    pid_t proc, spawn, giocatore,proiett;

    /* Strutture per la gestione dei nemici */
    int pr_rec[MAX_PROIETTILI][3] = {};
    int jump[ENEM_TEST + 1] = {1};
    int kill_pr[ENEM_TEST] = {};
    int helper = 0;
    int maxenemies = ENEM_TEST;
    int arrint[ENEM_TEST][7] = {}; //Contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    Player arr[ENEM_TEST] = {}; //Contiene le info di tutti in nemici, controllare enemyLV1 per piu info

    /* Flag, contatori e distanze */
    int i, coordinata, decremento, w = 0, k = 0, delta; //Contatori
    int y_spawner; //Indica in che ordinata andrà a spawnare il nemico
    int hit; // Flag se la navetta principale è stata colpita
    int player_started = 1; // Game-Start
    int invincibility = 0; // Flag e durata di invincibilità
    int killed = 0; //Indica i nemici uccisi
    int jumpbox = 6; // (>2) Distanza di rimbalzo
    int hitbox = 2; //Distanza dei caratteri dal centro
    int num_proiettili = 0;
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    for (i = 0; i < ENEM_TEST; i++)
    {
        pipe(tmp[i]);
        pipe(enemy_frame[i]);
    }
    
    
    


    giocatore = fork();
    switch (giocatore) {
        case 0:
            //  attenzione a non diminuire troppo il terzo valore,altrimenti ci potrebbero essere
            //  problemi di lettura input
            gestore_input(player_input, playerpipe, 25);
            exit(0);
            break;
        default:
            werase(w1);
            proc = fork();
            //creazione processo schermo-nemici
            switch (proc) {
                //spawn dei nemici,diventeranno tutti proccessi separati
                case 0:   //k mi serve per decidere quando spawnarli
                    for (coordinata=1;coordinata<maxenemies+1;coordinata++){
                        spawn = fork();
                        switch (spawn) {
                            case 0:        //variabili di spawn nemici
                                // per testare le due versioni commenta e decommenta
                                coordinata = (coordinata * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse y)
                                decremento = coordinata / (maxy-2); //Ogni volta che si supera il maxy viene decrementata la x
                                y_spawner = coordinata % (maxy-2); //Si prende il modulo per scegliere la coordinata dello sprite
                                decremento = (decremento * 3 * 2); //In modo da avere almeno uno sprite di stacco tra i nemici (Asse x)
                                enemyLV1_old(70-decremento, y_spawner, k,0, tmp[k], enemy_frame[k]);
                                //enemyLV1_new(70, coordinata * 3 * 2, k, 0, tmp, enemy_frame);
                                //Per i rimbalzi possiamo far gestire all'utente: 0 i nemici non collidono tra loro, 1 movimenti casuali

                                //se si chiudono bene non passano da questo exit,di norma si auto-terminano appena raggiungono la fine dello schermo
                                exit(-1);
                                break;
                            default:
                                k++;
                                break;
                        }
                    }
                    exit(0);
                    break;
                default:
                    //aspetto l'avvio dei nemici prima di iniziare la scrittura schermo
                    wait((int *) 0);

                    // finche non raggiungo il gameover,scrivo lo schermo
                    while (player_started) {
                        //prossimo frame,la clear in compenso all'erase non cancella lo schermo ma solo il buffer dello schermo
                        read(playerpipe[0], &player_input, sizeof(Bullet));
                        player.coordinata.x = player_input.x;
                        player.coordinata.y = player_input.y;
                        player.proiettile.ready = player_input.ready;

                        //helper ci fornisce una specie di angolo per sparare diagonalmente i proietili
                        if (player.proiettile.ready == PRONTO && num_proiettili < MAX_PROIETTILI)
                        {
                           ++num_proiettili;
                           proiett = fork();
                           switch (proiett)
                           {
                           case 0:
                               proiettile(player.coordinata.x,player.coordinata.y,bullet_p,bullet_ps);
                               exit(0);
                               break;
                           default:
                               break;
                           }
                           
                        }


                        // lettura pipes
                        for (i = 0; i < ENEM_TEST; i++) {
                            // per testare le versioni commenta e decommenta

                            /* new
                            read(tmp[0], &arr[i],sizeof(Player));
                            */

                            ///* old

                            if(arr[i].coordinata.y > -1){
                                read(tmp[i][0], &arrint[i][0],7*sizeof(int));
                                arr[i].coordinata.x = arrint[i][0];
                                arr[i].coordinata.y = arrint[i][1];
                                arr[i].proiettile.x = arrint[i][2];
                                arr[i].proiettile.y = arrint[i][3];
                                arr[i].proiettile.id = arrint[i][4];
                                arr[i].id = arrint[i][5];
                                arr[i].angolo = arrint[i][6];
                            }
                            //++i;
                            //*/
                        }
                        for(i=0;i < num_proiettili;i++){
                            read(bullet_p[0],pr_rec[i],3*sizeof(int));
                            proiettili[i].x = pr_rec[i][0];
                            proiettili[i].y = pr_rec[i][1];
                            proiettili[i].ready = pr_rec[i][2];
                        }
                    
                        // debug
                        /*
                        mvwprintw(w1, 10, 10, "%d,%d,%d",pr_rec[0][0],pr_rec[0][1],pr_rec[0][2]);
                        mvwprintw(w1, 10+1, 10, "%d,%d,%d",pr_rec[1][0],pr_rec[1][1],pr_rec[1][2]);

                        mvwprintw(w1,1,26,"id:%d X:%d Y:%d ",arrint[0][5],arrint[0][0],arrint[0][1]);
                        //*/
                        wclear(w1);
                        w = 0;
                        //controllo collisioni/rimbalzi
                        for (i = 0; i < ENEM_TEST; i++) {
                            if(arr[i].coordinata.y > -1){
                            ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                            for (w = i + 1; w < ENEM_TEST; w++) {
                                // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                                // attenzione a non ridurla troppo altrimenti ci potrbbero essere conflitti di sprite

                                // controllo distanza tra i due nemici                  controllo se sono nella stessa x e che abbiano direzione diversa
                                if(kill_pr[i] == 0 && kill_pr[w] == 0){
                                if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) < jumpbox) && arr[i].coordinata.x == arr[w].coordinata.x && arr[i].angolo !=
                                                                                                                                                          arr[w].angolo) {   // ulteriore controllo di direzione,attenzione quando si modifica qui

                                    if ((arr[i].coordinata.y < arr[w].coordinata.y && arr[i].angolo == 0 && arr[w].angolo == 1) ||
                                        (arr[i].coordinata.y > arr[w].coordinata.y && arr[i].angolo == 1 && arr[w].angolo == 0)) {
                                        // salvataggio dei processi che necessitano di rimbalzo,usando l'id dei processi
                                        jump[arr[w].id + 1] = 1;
                                        jump[arr[i].id + 1] = 1;
                                        // info di debug
                                        mvwprintw(w1, arr[i].id + 3, 30, "Hit");
                                        break;
                                    }
                                }
                                }
                            }
                            //collisione navetta/limite con nemico
                            if ((arr[i].coordinata.x <= 1) || (arr[i].coordinata.y == player.coordinata.y &&
                                                               arr[i].coordinata.x == player.coordinata.x) || life == 0) {
                                //questo killa definitivamente tutti i nemici,ma serve principalmente in caso di gameover
                                //jump e usata per inviare info ai processi,in questo caso li killa tutti
                                jump[0] = 0;
                                player_started = 0;
                            }
                            //stampa nemici
                            if(arr[i].coordinata.y%2==0 && arr[i].coordinata.y) {
                                if (arr[i].proiettile.id == 3){
                                    printnemicolv1_f1(arr[i].coordinata.x, arr[i].coordinata.y, w1);
                                }else{
                                    printnemicolv2_f1(arr[i].coordinata.x, arr[i].coordinata.y, w1);
                                }
                            } else {
                                if (arr[i].proiettile.id == 3){
                                    printnemicolv1_f2(arr[i].coordinata.x, arr[i].coordinata.y, w1);
                                }else{
                                    printnemicolv2_f2(arr[i].coordinata.x, arr[i].coordinata.y, w1);
                                }
                            }


                            mvwaddch(w1, arr[i].proiettile.y, arr[i].proiettile.x, '-');

                            //collisione navetta-proiettile_nemico
                            //Se la navetta non è nello stato di invincibilita
                            if (invincibility == 0) {
                                /* La prima condizione dell'if copre la parte sinistra e destra del cannone
                                 * La seconda condizione copre il cannone della navicella principale
                                 * Se un proiettile nemico colpisce una di queste 5 coordinate, viene
                                 * Abilitata la flag "hit" con valore true e reso il player invincibile */
                                for(delta=-2;delta<DIM-2;delta++) {
                                    if (  ( arr[i].proiettile.y == player.coordinata.y+delta &&
                                            arr[i].proiettile.x == player.coordinata.x-4) ||
                                          ( arr[i].proiettile.y == player.coordinata.y &&
                                            arr[i].proiettile.x == player.coordinata.x) ){
                                        mvwaddch(w1, player.coordinata.y, player.coordinata.x, 'X');
                                        hit=1;
                                        invincibility = 60;
                                    }
                                }
                            }

                            /* Se la navetta principale è stata colpita, la flag hit e impostata a 1
                             * Questo fa sì che il giocatore principale perda una vita e venga resettata la flag a 0. */
                            if(hit==1){
                                life--;
                                hit = 0;
                            }

                            //collisione proiettile-navetta_nemica //
                            for(w = 0;w < num_proiettili;++w){
                            if (proiettili[w].x == arr[i].coordinata.x && ((hitbox > proiettili[w].y - proiettili[w].ready - arr[i].coordinata.y &&
                                                            -hitbox < proiettili[w].y - proiettili[w].ready - arr[i].coordinata.y) ||
                                                            (-hitbox < proiettili[w].y + proiettili[w].ready - arr[i].coordinata.y &&
                                                            hitbox > proiettili[w].y + proiettili[w].ready - arr[i].coordinata.y))) {
                                //possiamo sfruttare questo meccanismo per spawnare nemici da lv 1 a lv 2,e in generale per togliere la vita ai nemici
                                jump[arr[i].id + 1] = -1;
                                mvwaddch(w1, arr[i].coordinata.y, arr[i].coordinata.x, '#');
                                //serve per ridurre i nemici nei vari counter
                                if(arr[i].proiettile.id == 1){
                                    ++killed;
                                    kill_pr[arr[i].id] = 1;
                                    arr[i].coordinata.y = -5;
                                } 
                                }
                            }
                            }
                        }
                        //stampa a schermo
                        //stampa proiettili navetta
                        //stampa navetta
                        invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                        print_info(player.proiettile.ready,life, w1,maxx);
                        for(i = 0;i < num_proiettili;i++)
                        {   if (proiettili[i].x == -1 && proiettili[i].y == -1)
                            {
                               --num_proiettili;
                               --i;
                            }else{
                            mvwprintw(w1, proiettili[i].y + proiettili[i].ready, proiettili[i].x, " o");
                            mvwprintw(w1, proiettili[i].y - proiettili[i].ready, proiettili[i].x, " o");
                            write(bullet_ps[1],&helper,sizeof(int));
                            }
                        }

                        /* altre info di debug

                        //printpl_info(w1,plarr);
                        ///*
                        i = 0;
                        while (i < ENEM_TEST + 1 )
                        {
                            mvwprintw(w1,i,21,"id:%d:%d ",i,jump[i]);
                            ++i;
                        }//*/

                        //sincronizzazione processi + invio info su rimbalzi/uccisioni
                        for (i = 0; i < ENEM_TEST; i++) {
                            if(arr[i].coordinata.y > -1){
                            write(enemy_frame[i][1], jump, (ENEM_TEST + 1) * sizeof(int));}
                            //mvwprintw(w1,i+1,32,"id:%d X:%d Y:%d vita:%d ",arr[i].id,arr[i].coordinata.x,arr[i].coordinata.y,arr[i].proiettile.id);
                            
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

    if (maxenemies==0){
        victory(w1, player.coordinata.x, player.coordinata.y);
    } else {
        game_over(w1, player.coordinata.x, player.coordinata.y);
    }
    //chiusura lettore input
    kill(player_input.id, SIGKILL);
    /*
    i = 0;p
    //chiusura pulita in caso di gameover per nemico che raggiunge il limite,necessita una revisione in modo che sia piu veloce
    while (i < maxenemies){
        read(tmp[0], arr[i], 7 * sizeof(int));
        ++i;
    }
    */
}
