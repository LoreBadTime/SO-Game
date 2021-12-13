#include "./process.h"


// Test nemici di livello 1
// x,y = coordinate.
// id = id nemico(per organizzazione)
// sender,receiver = pipes.
// direzione indica la direzione di partenza del nemico
int enemyLV1(int x,int y,int id,int direzione,int *sender,int *receiver){

    //Inizializzazione navicella nemica
    pid_t enemy;
    int p_enemy[2];
    pipe(p_enemy);
    int alive = VIVA; //Stato navicella nemica

    //Inizializzazione nemico,info del nemico
    Player nemico;
    nemico.proiettile.x=0;
    nemico.proiettile.y=0;
    nemico.proiettile.ready=0;
    nemico.coordinata.x=x;
    nemico.coordinata.y=y;
    nemico.angolo=direzione;
    nemico.id=id;

    //Risoluzione e pipe per inviare le coordinate
    int maxy,maxx;
    int send_info[7] = {};
    int rec[ENEM_TEST + 1] = {};
    close(receiver[1]);
    getmaxyx(stdscr, maxy, maxx);

    //Ciclo che gesitisce il nemico
    while (nemico.coordinata.x > 0 && alive)
    {
        //Ciclo che gestisce il rimbalzo
        while ((nemico.coordinata.y >= 0 && nemico.coordinata.y <= maxy-2))
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
                        nemico.coordinata.y--;
                    }
                    else
                    {
                        nemico.coordinata.y++;
                    }
                    if (write(p_enemy[1], &nemico, sizeof(Player)) <= 0)
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
                    if (read(p_enemy[0], &nemico, sizeof(Player)) <= 0)
                    {
                        mvprintw( maxy/2 , maxx/2 , "Error reading enemy");
                        refresh();
                    }
                    
                    /*send_info[0] = nemico.coordinata.x;
                    send_info[1] = nemico.coordinata.y;
                    send_info[2] = nemico.proiettile.x;
                    send_info[3] = nemico.proiettile.y;
                    send_info[4] = nemico.proiettile.ready;//getpid();
                    send_info[5] = id;
                    send_info[6] = direzione;*/
                    write(sender[1],&nemico,sizeof(Player));
                    read(receiver[0],rec,(ENEM_TEST + 1)*sizeof(int));
                    //la read serve per mettere il processo in waiting per il prossimo frame,altrimenti puo esserci un
                    //processo veloce che invia info piu velocemente rispetto ad altri

                    //killa navicella nemica
                    if (rec[0] == 0 || rec[id + 1] == -1){
                        nemico.coordinata.y = -1;
                        alive = UCCISA;
                    }
                    //dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
                    if(rec[id + 1]){
                        direzione = !direzione;
                        rec[id + 1] = 0;
                    }
                    //Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
                    if(nemico.proiettile.x <= -1 && (rand() % 101 == 0)){
                        nemico.proiettile.x = nemico.coordinata.x;
                        nemico.proiettile.y = nemico.coordinata.y;
                    }
                    break;
            }
        }

        --nemico.coordinata.x; //La navicella nemica avanza finchè non arriva alla x del player
        --nemico.coordinata.x;
        --nemico.coordinata.x;
        direzione = !direzione; //Cambio direzione navicella nemica (per il rimbalzo)

        //Incrementi delle coordinate.y per rientrare nel ciclo
        if (nemico.coordinata.y <= -1){
            nemico.coordinata.y++;
        }
        else{
            nemico.coordinata.y--;
        }
    }

    //Nel caso in cui arrivi alla fine,per non bloccare le pipes

    close(p_enemy[0]);
    close(p_enemy[1]);
    close(sender[1]);
    exit(0);
}


//Player contiente tutte le info del player
//sys_slownes e la velocita di gioco
void gestore_input(Player player,int *pipe,int sys_slownes){
    int c;
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    player.id = getpid();
    timeout(0);

    while (true){
        //meglio usare la getch,cosi non fa refresh allo schermo princiale
        c = getch();
        // se arriva l'input di lancio del proiettile ed esso si trova(per comodita implementativa) dietro la navetta
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
        if (player.proiettile.x > 1 && player.proiettile.ready == PRONTO)
        {
            // questi servono per regolare la velocita del proiettile della navicella + incrementi = + veloce
            ++player.proiettile.x;
        }
        // raggiunto un limite e se ancora attivo lo termino(si puo modificare il limite per velocizzare il tempo di spawn)
        if (player.proiettile.x > maxx && player.proiettile.ready == PRONTO)
        {
            player.proiettile.ready = 0;
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
        // ottenimento dati da keyboard
        if (c != ERR){   
                switch (c)
                {
                case KEY_UP:
                    if (player.coordinata.y > LARGHEZZA)
                    {
                        player.coordinata.y--;
                    }
                    break;
                case KEY_DOWN:
                    if (player.coordinata.y < maxy - LARGHEZZA)
                    {
                        player.coordinata.y++;
                    }
                    break;
                    // lancio proiettile
                case (int)' ':
                    player.proiettile.ready = PRONTO;
                default:
                    break;
                }
                while(c == getch()){;}         
        }
    write(pipe[1],&player,sizeof(Player));
    napms(sys_slownes);
    }
}


// stampa info player(non necessaria)
void printpl_info(WINDOW *w1,int *arr){
    int i = 0;
    mvwprintw(w1,0,15,"%d,%d,%d,%d,%d,%d,%d ",arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6]);
}
void clear_matrix(int arr[ENEM_TEST][7]){
    int i = 0;
    while (i < ENEM_TEST)
    {
        arr[i][0]=1000;
        arr[i][1]=1000;
        ++i;
    }
    
}

// Funzione principale
// Gestisce l'output a video
// Il parametro è la finestra di riferimento
void screen(WINDOW *w1){

	/* Inizializzazione giocatore */
	Player player;
	player.coordinata.x=5;
	player.coordinata.y=10;
	player.proiettile.x=0;
	player.proiettile.y=0;
	player.proiettile.ready=0;
	player.id=0;
	player.angolo=0;

    pid_t proc,spawn,giocatore;
    int tmp[2];
    int enemy_frame[2];
    int playerpipe[2];
    //matrice che contiene le info di tutti in nemici,controllare la funzione enemyLV1 per piu info
    int arr[ENEM_TEST][7] = {};
    //array che contiene alcune info da inviare ai nemici(tra cui il salto e l'uccisione)

    int jump[ENEM_TEST + 1] = {1}; 
    int kill_pr[ENEM_TEST] = {};
    int helper = 0;
    pipe(tmp);
    pipe(enemy_frame);
    pipe(playerpipe);
    int maxenemies = ENEM_TEST;
    int i = 0;
    int j = -1;
    int player_started = 1;
    int c;
    int w = 0;
    int k = 0;
    int invincibility = 0;
    int life = 3;
    int killed = 0;
    int jumpbox = 6; //>2
    int lol = 0;
    int hitbox = 2;
    //processo input    
    giocatore = fork();
    switch (giocatore)
    {
    case 0:
    //  attenzione a non diminuire troppo il terzo valore,altrimenti ci potrebbero essere
    //  problemi di lettura input
        gestore_input(player,playerpipe,75);
        exit(0);
        break;
    default:
        break;
    }
    wclear(w1);
    proc = fork();
    //creazione processo schermo-nemici
    switch (proc)
    {
        //spawn dei nemici,diventeranno tutti proccessi separati
        case 0:   //k mi serve per decidere quando spawnarli
            while (k < maxenemies)
            {
                spawn = fork();
                switch (spawn)
                {
                    case 0:        //variabili di spawn nemici
                        enemyLV1(70, k*3*2, k, 0, tmp, enemy_frame);
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
            // finche non raggiungo il gameover,scrivo lo schermo
            while (player_started)
            {   
                wclear(w1);//prossimo frame,la clear in compenso all'erase non cancella lo schermo ma solo il buffer dello schermo
                read(playerpipe[0],&player,sizeof(Player));
                //helper ci fornisce una specie di angolo per sparare diagonalmente i proietili
                helper = player.angolo;
                //lettura pipes per mettere tutto in una matrice
                for(i=0;i < maxenemies;i++){  
                    read(tmp[0], arr[i], 7 * sizeof(int));
                }
                //possibile idea per velocizzazione
                
                //lettura per rimbalzi

                // debug
                /*
                mvwprintw(w1,1,26,"id:%d X:%d Y:%d ",arr[0][5],arr[0][0],arr[0][1]);
                mvwprintw(w1,2,26,"id:%d X:%d Y:%d ",arr[1][5],arr[1][0],arr[1][1]);
                //*/
                w = 0;
                //controllo collisioni
                for(i=0;i < maxenemies;i++){   
                    ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                    for(w=i+1;w < maxenemies;w++){   
			// modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                        // attenzione a non ridurla troppo altrimenti ci potrbbero essere conflitti di sprite
                        
                        // controllo distanza tra i due nemici                  controllo se sono nella stessa x e che abbiano direzione diversa
                        if((abs(abs(arr[i][1]) - abs(arr[w][1])) < jumpbox) && arr[i][0] == arr[w][0] && arr[i][6] != arr[w][6])
                        {   // ulteriore controllo di direzione,attenzione quando si modifica qui

                            if ((arr[i][1] < arr[w][1] && arr[i][6] == 0 && arr[w][6] == 1) || (arr[i][1] > arr[w][1] && arr[i][6] == 1 && arr[w][6] == 0))
                            {
                                // salvataggio dei processi che necessitano di rimbalzo,usando l'id dei processi
                                jump[arr[w][5] + 1] = 1;
                                jump[arr[i][5] + 1] = 1;
                                // info di debug
                                //mvwprintw(w1, arr[i][5] + 3, 30, "Hit");
                                break;
                            }
                        }
                    }
                    //*/
                    //collisione navetta/limite con nemico
                    if ((arr[i][0] <= 1) || (arr[i][1] == player.coordinata.y && arr[i][0] == player.coordinata.x) || life == 0){
                        //questo killa definitivamente tutti i nemici,ma serve principalmente in caso di gameover
                        //jump e usata per inviare info ai processi,in questo caso li killa tutti
                        jump[0] = 0;
                        player_started = 0;
                    }
                    //stampa nemici
                    if(arr[i][1] % 2 == 0){
                        printnemicolv1_f1(arr[i][0], arr[i][1], w1);
                    }else{
                        printnemicolv1_f2(arr[i][0], arr[i][1], w1);
                    }
                    
                    mvwaddch(w1,arr[i][3], arr[i][2], '-');
                    //collisione navetta-proiettile_nemico
                    if (invincibility == 0){
                        if(arr[i][3] == player.coordinata.y && arr[i][2] == player.coordinata.x){
                            mvwaddch(w1,player.coordinata.y, player.coordinata.x, 'X');
                            --life;
                            invincibility = 120;
                        }
                    }

                    //collisione proiettile-navetta_nemica //
                    if(player.proiettile.x == arr[i][0] && ((hitbox > player.proiettile.y-helper-arr[i][1] && -hitbox < player.proiettile.y-helper-arr[i][1]) || (-hitbox < player.proiettile.y+helper-arr[i][1] && hitbox > player.proiettile.y+helper-arr[i][1] ))){
                        //possiamo sfruttare questo meccanismo per spawnare nemici da lv 1 a lv 2,e in generale per togliere la vita ai nemici
                        jump[arr[i][5] + 1] = -1;
                        mvwaddch(w1,arr[i][1], arr[i][0], 'X');
                        //serve per ridurre i nemici nei vari counter
                        ++killed;
                    }
                }
                //stampa a schermo
                //stampa proiettili navetta
                mvwaddch(w1,player.proiettile.y+helper, player.proiettile.x, 'o');
                mvwaddch(w1,player.proiettile.y-helper, player.proiettile.x, 'o');
                //stampa navetta
                invincibility = print_nave(invincibility,w1,player.coordinata.x,player.coordinata.y);
                print_vita(life,w1);

                /* altre info di debug
                
                //printpl_info(w1,plarr);
                i = 0;
                while (i < ENEM_TEST)
                {
                    mvwprintw(w1,i+1,32,"id:%d X:%d Y:%d direzione:%d ",arr[i][5],arr[i][0],arr[i][1],arr[i][6]);
                    ++i;
                }//
                i = 0;
                while (i < ENEM_TEST + 1 )
                {
                    mvwprintw(w1,i,21,"id:%d:%d ",i,jump[i]);
                    ++i;
                }*/

                //sincronizzazione processi + invio info su rimbalzi/uccisioni
                for (i=0;i < maxenemies;i++){
                    write(enemy_frame[1],jump,(ENEM_TEST + 1)*sizeof(int));
                }

                //reset dei rimbalzi,necessario
                for(i=1;i <ENEM_TEST + 1;i++){
                    jump[i] = 0;
                }

                //riduzione cicli necessari generali,migliorabile(maxenemies = maxenemies - killed;)
                while (killed)
                {  
                    --killed;
                    --maxenemies;
                }
                wrefresh(w1);
            }
    }

    game_over(w1,player.coordinata.x, player.coordinata.y);
    //chiusura lettore input
    kill(player.id,SIGKILL);
    /*
    i = 0;
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
