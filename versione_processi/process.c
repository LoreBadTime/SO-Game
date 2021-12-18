#include "./process.h"

int enemyLV1_old(int x,int y,int id,int direzione,int *sender,int *receiver){

    //Inizializzazione navicella nemica
    pid_t enemy;
    int p_enemy[2];
    pipe(p_enemy);
    int alive = 1; //Stato navicella nemica
    //int direzione = 1; //Direzione navicella nemica, 1 = Da alto in basso, 0 = Da basso in alto

    //Inizializzazione nemico,info del nemico
    Navetta_Nemica nemico;
    nemico.proiettile.x=0;
    nemico.proiettile.y=0;
    nemico.proiettile.ready=0;
    nemico.navnemica.x=x;
    nemico.navnemica.y=y;
    int decremento = 0;
    //Risoluzione e pipe per inviare le coordinate
    int maxy,maxx;
    int send_info[7] = {};
    int rec[ENEM_TEST + 1] = {};
    close(receiver[1]);
    getmaxyx(stdscr, maxy, maxx);
    //Ciclo che gesitisce il nemico
    while (nemico.navnemica.x > 0 && alive)
    {
        //Ciclo che gestisce il rimbalzo
        while (nemico.navnemica.y >= 2 && nemico.navnemica.y <= maxy-3)
        {
            //Creazione processo: (P)schermo - (C)navicella/proiettile
            enemy = fork();
            switch (enemy){
                case 0:
                    //eliminato processo inutile del proiettile
                    //possibile eliminazione secondaria dato che anche qui non si fa altro che decrementare
                    --nemico.proiettile.x;
                    close(p_enemy[0]);
                    if (direzione)
                    {
                        nemico.navnemica.y--;
                    }
                    else
                    {
                        nemico.navnemica.y++;
                    }
                    //bisogna trovare i valori ideali di decremento(il 9)
                    if (decremento < 9)
                    {
                        if (direzione)
                        {
                            nemico.navnemica.y++;
                        }
                        else
                        {
                            nemico.navnemica.y--;
                        }
                        
                    }
                    if (write(p_enemy[1], &nemico, sizeof(Navetta_Nemica)) <= 0)
                    {
                        mvprintw(maxy / 2, maxx / 2, "Error writing enemy");
                        refresh();
                    }
                    close(p_enemy[1]);
                    exit(0);
                    break;
                    //In caso di errore della fork()
                case -1:
                    refresh();
                    exit(-1);
                    break;

                    //Comunicazione delle info tramite pipe
                default:
                    // rallentatore di movimento,modificando la condizione possiamo decidere di quanto rallentare i nemici
                    // permettendci di impostare velocita maggiori di fps 
                    ++decremento;
                    if(decremento == 10){
                        decremento = 0;
                    }

                    if (read(p_enemy[0], &nemico, sizeof(Navetta_Nemica)) <= 0)
                    {
                        mvprintw( maxy/2 , maxx/2 , "Error reading enemy");
                        refresh();
                    }
                    //algoritmo per rallent
                    
                    
                    send_info[0] = nemico.navnemica.x;
                    send_info[1] = nemico.navnemica.y;
                    send_info[2] = nemico.proiettile.x;
                    send_info[3] = nemico.proiettile.y;
                    send_info[4] = getpid();
                    send_info[5] = id;
                    send_info[6] = direzione;
                    write(sender[1],send_info,7*sizeof(int));
                    read(receiver[0],rec,(ENEM_TEST + 1)*sizeof(int));
                    //la read serve per mettere il processo in waiting per il prossimo frame,altrimenti puo esserci un
                    //processo veloce che invia info piu velocemente rispetto ad altri

                    //killa navicella nemica
                    if (rec[0] == 0 || rec[id + 1] == -1){
                        nemico.navnemica.y = -1;
                        alive = 0;
                    }
                    //dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
                    if(rec[id + 1]){
                        direzione = !direzione;
                        rec[id + 1] = 0;
                    }
                    //Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
                    if(nemico.proiettile.x <= -1 && (rand() % 2 == 0)){
                        nemico.proiettile.x = nemico.navnemica.x;
                        nemico.proiettile.y = nemico.navnemica.y;
                    }
                    break;
            }
        }
        --nemico.navnemica.x; //La navicella nemica avanza finchè non arriva alla x del player
        --nemico.navnemica.x;
        --nemico.navnemica.x;
        direzione = !direzione; //Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (nemico.navnemica.y <= 1){
            nemico.navnemica.y++;
        }
        if (nemico.navnemica.y >= maxy-2){
            nemico.navnemica.y--;
        }
    }

    //Nel caso in cui arrivi alla fine,per non bloccare le pipes

    close(p_enemy[0]);
    close(p_enemy[1]);
    close(sender[1]);
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
                if (nemico.proiettile.x <= -1 && (rand() % 101 == 0)) {
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
                    if (player.y > LARGHEZZA) {
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
    Player player;
    player.coordinata.x = 5;
    player.coordinata.y = 10;
    player.proiettile.x = 0;
    player.proiettile.y = 0;
    player.proiettile.ready = 0;
    player.id = 0;
    player.angolo = 0;

    Bullet player_input;
    player_input.x = player.coordinata.x;
    player_input.y = player.coordinata.y;
    player_input.ready = 0;

    pid_t proc, spawn, giocatore;
    int tmp[2];
    int enemy_frame[2];
    int playerpipe[2];
    //matrice che contiene le info di tutti in nemici, controllare la funzione enemyLV1 per piu info
    Player arr[ENEM_TEST] = {};
    int arrint[ENEM_TEST][7] = {};
    //array che contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    int jump[ENEM_TEST + 1] = {1};
    int kill_pr[ENEM_TEST] = {};
    int helper = 0;
    pipe(tmp);
    pipe(enemy_frame);
    pipe(playerpipe);
    int maxenemies = ENEM_TEST;
    int i = 0;
    int j = -1;
    int hit;
    int player_started = 1;
    int c;
    int coordinata;
    int w = 0;
    int k = 0;
    int delta = 0;
    int p=0;
    int invincibility = 0;
    int life = 3;
    int killed = 0;
    int jumpbox = 6; //>2
    int hitbox = 2;
    //processo input
    giocatore = fork();
    switch (giocatore) {
        case 0:
            //  attenzione a non diminuire troppo il terzo valore,altrimenti ci potrebbero essere
            //  problemi di lettura input
            gestore_input(player_input, playerpipe, 50);
            exit(0);
            break;
        default:
            break;
    }
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

                        enemyLV1_old(70, coordinata * 3 * 2, k,0, tmp, enemy_frame);
                        //enemyLV1_new(70, coordinata * 3 * 2, k, 0, tmp, enemy_frame);

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
                wclear(w1);//prossimo frame,la clear in compenso all'erase non cancella lo schermo ma solo il buffer dello schermo
                read(playerpipe[0], &player_input, sizeof(Bullet));
                player.coordinata.x = player_input.x;
                player.coordinata.y = player_input.y;
                player.proiettile.ready = player_input.ready;

                //helper ci fornisce una specie di angolo per sparare diagonalmente i proietili
                if (player.proiettile.ready == PRONTO && player.proiettile.x < player.coordinata.x)
                {
                    // lo lancio
                    // prendo le x e le y della navetta e le assegno al proiettile
                    player.proiettile.y = player.coordinata.y;
                    player.proiettile.x = player.coordinata.x;
                    // poi sparo
                    ++player.proiettile.x;
                }
                // una volta sparato lui avanza finche possibile
                if (player.proiettile.x > 1)
                {
                    // questi servono per regolare la velocita del proiettile della navicella + incrementi = + veloce
                    ++player.proiettile.x;
                }
                // raggiunto un limite e se ancora attivo lo termino(si puo modificare il limite per velocizzare il tempo di spawn)
                if (player.proiettile.x > maxx)
                {
                    player.proiettile.x = 0;
                    player.proiettile.y = 0;
                    player.angolo = 0;
                }
                // questo meccanismo permette di bloccare anche al lancio di un proiettile per volta,ma e comunque modificabile

                // modifica di come il proiettile ha la sua traiettoria diagonale
                if (player.proiettile.x % DIAGONALE == 1)
                {
                    ++player.angolo;
                }
                helper = player.angolo;
                // lettura pipes per mettere tutto in una matrice
                for (i = 0; i < maxenemies; i++) {
                    // per testare le versioni commenta e decommenta

                    /* new
                    read(tmp[0], &arr[i],sizeof(Player));
                    */
                   
                    ///* old
                    read(tmp[0], arrint[i],7*sizeof(int));
                    arr[i].coordinata.x = arrint[i][0];
                    arr[i].coordinata.y = arrint[i][1];
                    arr[i].proiettile.x = arrint[i][2];
                    arr[i].proiettile.y = arrint[i][3];
                    //send_info[4] = getpid();
                    
                    arr[i].id = arrint[i][5];
                    arr[i].angolo = arrint[i][6];
                    //*/
                }
                //possibile idea per velocizzazione

                //lettura per rimbalzi

                // debug
                /*
                mvwprintw(w1,1,26,"id:%d X:%d Y:%d ",arrint[0][5],arrint[0][0],arrint[0][1]);
                //*/
                w = 0;
                //controllo collisioni
                for (i = 0; i < maxenemies; i++) {
                    ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                    for (w = i + 1; w < maxenemies; w++) {
                        // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                        // attenzione a non ridurla troppo altrimenti ci potrbbero essere conflitti di sprite

                        // controllo distanza tra i due nemici                  controllo se sono nella stessa x e che abbiano direzione diversa
                        if ((abs(abs(arr[i].coordinata.y) - abs(arr[w].coordinata.y)) < jumpbox) && arr[i].coordinata.x == arr[w].coordinata.x && arr[i].angolo !=
                                                                                                          arr[w].angolo) {   // ulteriore controllo di direzione,attenzione quando si modifica qui

                            if ((arr[i].coordinata.y < arr[w].coordinata.y && arr[i].angolo == 0 && arr[w].angolo == 1) ||
                                (arr[i].coordinata.y > arr[w].coordinata.y && arr[i].angolo == 1 && arr[w].angolo == 0)) {
                                // salvataggio dei processi che necessitano di rimbalzo,usando l'id dei processi
                                jump[arr[w].id + 1] = 1;
                                jump[arr[i].id + 1] = 1;
                                // info di debug
                                //mvwprintw(w1, arr[i].id + 3, 30, "Hit");
                                break;
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
                    if(arr[i].coordinata.y%2==0) {
                        printnemicolv1_f1(arr[i].coordinata.x, arr[i].coordinata.y, w1);
                    } else {
                        printnemicolv1_f2(arr[i].coordinata.x, arr[i].coordinata.y, w1);
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
                    if (player.proiettile.x == arr[i].coordinata.x && ((hitbox > player.proiettile.y - helper - arr[i].coordinata.y &&
                                                              -hitbox < player.proiettile.y - helper - arr[i].coordinata.y) ||
                                                             (-hitbox < player.proiettile.y + helper - arr[i].coordinata.y &&
                                                              hitbox > player.proiettile.y + helper - arr[i].coordinata.y))) {
                        //possiamo sfruttare questo meccanismo per spawnare nemici da lv 1 a lv 2,e in generale per togliere la vita ai nemici
                        jump[arr[i].id + 1] = -1;
                        mvwaddch(w1, arr[i].coordinata.y, arr[i].coordinata.x, '#');
                        //serve per ridurre i nemici nei vari counter
                        ++killed;
                    }
                }
                //stampa a schermo
                //stampa proiettili navetta
                mvwaddch(w1, player.proiettile.y + helper, player.proiettile.x, 'o');
                mvwaddch(w1, player.proiettile.y - helper, player.proiettile.x, 'o');
                //stampa navetta
                invincibility = print_nave(invincibility, w1, player.coordinata.x, player.coordinata.y);
                print_vita(life, w1);

                /* altre info di debug

                //printpl_info(w1,plarr);
                i = 0;
                while (i < ENEM_TEST)
                {
                    mvwprintw(w1,i+1,32,"id:%d X:%d Y:%d direzione:%d ",arr[i].id,arr[i].coordinata.x,arr[i].coordinata.y,arr[i].angolo);
                    ++i;
                }//
                i = 0;
                while (i < ENEM_TEST + 1 )
                {
                    mvwprintw(w1,i,21,"id:%d:%d ",i,jump[i]);
                    ++i;
                }*/

                //sincronizzazione processi + invio info su rimbalzi/uccisioni
                for (i = 0; i < maxenemies; i++) {
                    write(enemy_frame[1], jump, (ENEM_TEST + 1) * sizeof(int));
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

/*
//collisione fine_schermo-nemici/nemico-navetta,gameover
*/

