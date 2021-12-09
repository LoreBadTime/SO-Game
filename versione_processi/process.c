#include "./process.h"

int plarr[5] = {5,10,0,0,0};
//              x,y,xproiettile,yproiettile,proiettile disponibile/lanciato

// Test nemici di livello 1
// x,y = coordinate.
// slownes = velocità di gioco.
// sender,receiver = pipes.
int enemyLV1(int x,int y,int slownes,int *sender,int *receiver){

    //Inizializzazione navicella nemica
    oggetto navnemica;
    navnemica.x=x;
    navnemica.y=y;
    pid_t enemy;
    int p_enemy[2];
    pipe(p_enemy);
    int alive = 1; //Stato navicella nemica
    int direzione = 1; //Direzione navicella nemica, 1 = Da alto in basso, 0 = Da basso in alto

    //Inizializzazione proiettile
    Bullet proiettile;
    proiettile.x=0;
    proiettile.y=0;
    proiettile.ready=0;
    pid_t bullet;
    int p_bullet[2];
    pipe(p_bullet);

    //Risoluzione e pipe per inviare le coordinate
    int maxy,maxx;
    int send_info[5] = {};
    int rec = 0;

    write(p_bullet[1], &proiettile, sizeof(Bullet));
    close(receiver[1]);
    getmaxyx(stdscr, maxy, maxx);

    //Ciclo che gesitisce il nemico
    while (navnemica.x > 0 && alive)
    {
        //Ciclo che gestisce il rimbalzo
        while ((navnemica.y >= 0 && navnemica.y <= maxy))
        {
            //Creazione processo: (P)schermo - (C)navicella/proiettile
            enemy = fork();
            switch (enemy){
                case 0:
                    //Creazione sottoprocessi: (P)navicella - (C)proiettile
                    bullet = fork();
                    switch (bullet)
                    {
                        //Processo proiettile, serve per aquisire separatamente le coordinate del proiettile
                        case 0:
                            close(p_bullet[0]);
                            --proiettile.x;
                            if(write(p_bullet[1], &proiettile, sizeof(Bullet)) <= 0){
                                mvprintw( maxy/2 , maxx/2 , "Error writing bullet");
                                refresh();
                            }
                            close(p_bullet[1]);
                            exit(0);
                            break;

                            //Processo navicella,gestisce il rimbalzo e il movimento della navicella,potrebbe essere ottimizzabile
                        default:
                            close(p_enemy[0]);
                            if (direzione == 1){
                                navnemica.y--;
                            }
                            else{
                                navnemica.y++;
                            }
                            if (write(p_enemy[1], &navnemica , sizeof(oggetto)) <= 0){
                                mvprintw( maxy/2 , maxx/2 , "Error writing enemy");
                                refresh();
                            }
                            close(p_enemy[1]);
                            exit(0);
                            break;
                    }

                    //In caso di errore della fork()
                case -1:
                    refresh();
                    exit(-1);
                    break;

                    //Comunicazione delle info tramite pipe
                default:
                    if (read(p_enemy[0], &navnemica, sizeof(oggetto)) <= 0)
                    {
                        mvprintw( maxy/2 , maxx/2 , "Error reading enemy");
                        refresh();
                    }
                    if(read(p_bullet[0], &proiettile, sizeof(Bullet)) <= 0){
                        mvprintw( maxy/2 , maxx/2 , "Error reading bullet");
                        refresh();
                    }
                    send_info[0] = navnemica.x;
                    send_info[1] = navnemica.y;
                    send_info[2] = proiettile.x;
                    send_info[3] = proiettile.y;
                    send_info[4] = getpid();

                    write(sender[1],send_info,5*sizeof(int));
                    read(receiver[0],&rec,sizeof(int));
                    if (rec == 0){
                        navnemica.y = -1;
                        alive = 0;
                    }

                    napms(slownes);

                    //Ottenimento info per lanciare il processo proiettile,se lo facciamo comunicare possiamo decidere il numero di proiettili generati
                    if(proiettile.x <= -1 && rec < 4){
                        proiettile.x = navnemica.x;
                        proiettile.y = navnemica.y;
                    }
                    break;
            }
        }

        --navnemica.x; //La navicella nemica avanza finchè non arriva alla x del player
        direzione = !direzione; //Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (navnemica.y <= -1){
            navnemica.y++;
        }
        else{
            navnemica.y--;
        }
    }

    //Nel caso in cui arrivi alla fine
    send_info[0] = -1;
    send_info[1] = -1;
    send_info[2] = -1;
    send_info[3] = -1;
    send_info[4] = getpid();
    write(sender[1],send_info,5*sizeof(int));
    close(p_enemy[0]);
    close(p_enemy[1]);
    close(p_bullet[0]);
    close(p_bullet[1]);
    close(sender[1]);
    exit(0);
}

//implementata mediante thread,necessita reimplementazione processi
void *getinput(){
    int c;
    int maxy,maxx;
    while (true){
        //meglio usare la getch,cosi non fa refresh allo schermo princiale
        c = getch();
        getmaxyx(stdscr, maxy, maxx);
        if (c != ERR)
        {
            switch (c)
            {
                case KEY_UP:
                    plarr[1] -= 1;
                    break;
                case KEY_DOWN:
                    plarr[1] += 1;
                    break;
                    //lancio proiettile
                case (int)' ':
                    plarr[4] = 1;
                default:
                    break;
            }
        }
    }
}

void screen(WINDOW *w1){
    pid_t proc,spawn,player;
    // purtroppo questa cosa deve essere asincrona,altrimenti la performance crolla completamente,per questo ho usato i thread qui per ottenere gli input
    // servirebbe chiedere ai prof che fare,altrimenti bisogna trovare un metodo buono usando i processi,riciclabile per la versione con i thread
    pthread_t inpt_id;
    int tmp[2];
    int enemy_frame[2];
    int playerpipe[2];
    int arr[5];
    int send = 1;
    int helper = 0;
    pipe(tmp);
    pipe(enemy_frame);
    pipe(playerpipe);
    int maxenemies = ENEM_TEST;
    int i = 0;
    int j = -1;
    int player_started = 1;
    int c;
    int k = 0;
    int invincibility = 0;
    // purtoppo per adesso 6,dato che ad ogni fps viene contata la posizione,come workaround ho usato 6 per il numero di vite
    // un fix con invincibilta della navetta potrebbe risolvere questo problema
    int life = 3;
    //creazione thread per l'input del giocatore
    pthread_create(&inpt_id, NULL, getinput, NULL);
    wclear(w1);
    proc = fork();
    //screazione processo schermo-nemici
    switch (proc)
    {
        //spawn dei nemici,diventeranno tutti proccessi separati
        case 0:   //k mi serve per decidere quando spawnarli
            while (k < maxenemies)
            {
                spawn = fork();
                switch (spawn)
                {
                    case 0:        //variabili di spawn
                        enemyLV1(70, 20 + (2 * k), 30, tmp, enemy_frame);
                        //se si chiudono bene non passano da questo exit,di norma si auto-terminano appena raggiungono la fine dello schermo
                        exit(-1);
                        break;
                    default:
                        ++k;
                        break;
                }
            }
            exit(0);
            break;
        default:
            //aspetto l'avvio dei nemici prima di iniziare la scrittura schermo
            wait((int *)0);
            // finche non raggiungo il gameover scrivo lo schermo
            while (player_started)
            {
                wclear(w1);//prossimo frame,la clear in compenso all'erase non cancella lo schermo ma solo il buffer dello schermo
                i = 0;
                while (i < life)
                {
                    mvwaddch(w1,0,3+i, '>');
                    ++i;
                }

                //mvwaddch(w1,plarr[1], plarr[0], '>');
                //printnavetta(plarr[0],plarr[1],w1);
                i = 0;
                if(invincibility){
                    --invincibility;
                    if(invincibility % 10 < 7){
                        printnavetta(plarr[0],plarr[1],w1);
                    }
                }else{
                    printnavetta(plarr[0],plarr[1],w1);
                }
                //se arriva l'input di lancio del proiettile ed esso si trova(per comodita implementativa) dietro la navetta
                if(plarr[4] == 1 && plarr[2] < plarr[0]){
                    //lo lancio
                    //prendo le x e le y della navetta e le assegno al proiettile
                    plarr[3] = plarr[1];
                    plarr[2] = plarr[0];
                    //poi sparo
                    ++plarr[2];
                }
                //una volta sparato lui avanza finche possibile
                if (plarr[2] > 1 && plarr[4] == 1)
                {
                    //questi servono per regolare la velocita del proiettile della navicella + incrementi = + veloce
                    ++plarr[2];
                    ++plarr[2];
                    ++plarr[2];
                }
                //raggiunto un limite e se ancora attivo lo termino(si puo modificare il limite per velocizzare il tempo di spawn)
                if(plarr[2] > 83 && plarr[4] == 1){
                    plarr[4] = 0;
                    plarr[2] = 0;
                    plarr[3] = 0;
                    helper = 0;
                }
                //questo meccanismo permette di bloccare anche al lancio di un proiettile per volta,ma e comunque modificabile

                //modifica di come il proiettile ha la sua traiettoria diagonale
                if(plarr[2] % 4 == 1){
                    helper++;
                }
                mvwaddch(w1,plarr[3]+helper, plarr[2], 'o');
                mvwaddch(w1,plarr[3]-helper, plarr[2], 'o');
                while (i < maxenemies){
                    read(tmp[0], arr, 5 * sizeof(int));
                    //collisione fine_schermo-nemici/nemico-navetta,gameover
                    if ((arr[0] <= 1) || (arr[1] == plarr[1] && arr[0] == plarr[0]) || life == 0){
                        send = 0;
                        player_started = 0;
                    }
                    mvwaddch(w1,arr[1], arr[0], '<');
                    mvwaddch(w1,arr[3], arr[2], '-');
                    //collisione navetta-proiettile_nemico
                    if (invincibility == 0){
                        if(arr[3] == plarr[1] && arr[2] == plarr[0]){
                            mvwaddch(w1,plarr[1], plarr[0], 'X');
                            --life;
                            invincibility = 120;
                        }
                        //qua possiamo dare il tempo di invincibilita
                    }

                    //collisione proiettile-navetta_nemica
                    if(plarr[2] == arr[0] && (plarr[3]-helper == arr[1] || plarr[3]+helper == arr[1])){
                        //magari si puo trovare un modo piu elegante per killare il processo nemico
                        kill(arr[4],SIGKILL);
                        mvwaddch(w1,arr[1], arr[0], 'X');
                        --maxenemies;
                        --i;
                    }
                    ++i;
                }
                i = 0;
                wrefresh(w1);

                //mandando a 0 send invio a tutti i processi l'autokill,di norma pero serve per sincronizzare i processi con lo schermo
                //(in modo da evitare lo sfarfallio di posizioni) potrebbe inviare anche altre info(come per limitare il numero di proiettili nemici)
                while (i < maxenemies){
                    write(enemy_frame[1],&send,sizeof(int));
                    ++i;
                }

            }
    }
    game_over(w1,plarr[0], plarr[1]);
    i = 0;
    //chiusura pulita in caso di gameover per nemico che raggiunge il limite,necessita una revisione in modo che sia piu veloce
    while (i < maxenemies){
        read(tmp[0], arr, 5 * sizeof(int));
        ++i;
    }

}
