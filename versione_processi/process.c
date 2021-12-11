#include "./process.h"



// Test nemici di livello 1
// x,y = coordinate.
// slownes = velocità di gioco.
// sender,receiver = pipes.

int enemyLV1(int x,int y,int slownes,int id,int direzione,int *sender,int *receiver){

    //Inizializzazione navicella nemica
    pid_t enemy;
    int p_enemy[2];
    pipe(p_enemy);
    int alive = 1; //Stato navicella nemica
    //int direzione = 1; //Direzione navicella nemica, 1 = Da alto in basso, 0 = Da basso in alto

    //Inizializzazione nemico,ho incluso tutte le info qua
    Navetta_Nemica nemico;
    nemico.proiettile.x=0;
    nemico.proiettile.y=0;
    nemico.proiettile.ready=0;
    nemico.navnemica.x=x;
    nemico.navnemica.y=y;
    pid_t bullet;


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
        while ((nemico.navnemica.y >= 0 && nemico.navnemica.y <= maxy))
        {
            //Creazione processo: (P)schermo - (C)navicella/proiettile
            enemy = fork();
            switch (enemy){
                case 0:
                    //eliminato processo inutile
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
                    if (read(p_enemy[0], &nemico, sizeof(Navetta_Nemica)) <= 0)
                    {
                        mvprintw( maxy/2 , maxx/2 , "Error reading enemy");
                        refresh();
                    }
                    
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
                    if (rec[0] == 0){
                        nemico.navnemica.y = -1;
                        alive = 0;
                    }
                    //dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
                    if(rec[id + 1]){
                        direzione = !direzione;
                        rec[id + 1] = 0;
                    }
                    napms(slownes);
                    //Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
                    if(nemico.proiettile.x <= -1 && (rand() % 101 == 0)){
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
        if (nemico.navnemica.y <= -1){
            nemico.navnemica.y++;
        }
        else{
            nemico.navnemica.y--;
        }
    }

    //Nel caso in cui arrivi alla fine,per non bloccare le pipes
    send_info[0] = -1;
    send_info[1] = -1;
    send_info[2] = -1;
    send_info[3] = -1;
    send_info[4] = getpid();
    send_info[5] = id;
    send_info[6] = -1;
    write(sender[1],send_info,7*sizeof(int));
    close(p_enemy[0]);
    close(p_enemy[1]);
    close(sender[1]);
    exit(0);
}

//necessita migliori tempisticheprocessi
void gestore_input(int *plarr,int *pipe){
    int tmp = 0;
    int c;
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    plarr[5] = getpid();
    timeout(0);
    while (true){
        //meglio usare la getch,cosi non fa refresh allo schermo princiale
        c = getch();
        // se arriva l'input di lancio del proiettile ed esso si trova(per comodita implementativa) dietro la navetta
        if (plarr[4] == 1 && plarr[2] < plarr[0])
        {
            // lo lancio
            // prendo le x e le y della navetta e le assegno al proiettile
            plarr[3] = plarr[1];
            plarr[2] = plarr[0];
            // poi sparo
            ++plarr[2];
        }
        // una volta sparato lui avanza finche possibile
        if (plarr[2] > 1 && plarr[4] == 1)
        {
            // questi servono per regolare la velocita del proiettile della navicella + incrementi = + veloce
            ++plarr[2];
            ++plarr[2];
            ++plarr[2];
        }
        // raggiunto un limite e se ancora attivo lo termino(si puo modificare il limite per velocizzare il tempo di spawn)
        if (plarr[2] > 83 && plarr[4] == 1)
        {
            plarr[4] = 0;
            plarr[2] = 0;
            plarr[3] = 0;
            plarr[6] = 0;
        }
        // questo meccanismo permette di bloccare anche al lancio di un proiettile per volta,ma e comunque modificabile

        // modifica di come il proiettile ha la sua traiettoria diagonale
        if (plarr[2] % 4 == 1)
        {
            ++plarr[6];
        }
        if (c != ERR){   
                switch (c)
                {
                case KEY_UP:
                    if (plarr[1] > 3)
                    {
                        plarr[1] -= 1;
                    }
                    break;
                case KEY_DOWN:
                    if (plarr[1] < maxy - 3)
                    {
                        plarr[1] += 1;
                    }
                    break;
                    // lancio proiettile
                case (int)' ':
                    plarr[4] = 1;
                default:
                    break;
                }         
        }
    write(pipe[1],plarr,7*sizeof(int));
    //purtroppo e necessario aspettare,altrimenti l'input è rallentato
    napms(35);
    }
}
// stampa info player(non necessaria)
void printpl_info(WINDOW *w1,int *arr){
    int i = 0;
    mvwprintw(w1,0,15,"%d,%d,%d,%d,%d,%d,%d ",arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6]);
}

    

void screen(WINDOW *w1){
    //              x,y,xproiettile,yproiettile,proiettile disponibile/lanciato,id processo input,modifica angolo proiettile
    int plarr[7] = {5,10,0,0,0,0,0};
    pid_t proc,spawn,player;
   //pthread_t inpt_id;
    int tmp[2];
    int enemy_frame[2];
    int playerpipe[2];
    int arr[ENEM_TEST][7] = {};
    int jump[ENEM_TEST + 1] = {1}; 
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
    //processo input    
    player = fork();
    switch (player)
    {
    case 0:
        gestore_input(plarr,playerpipe);
        exit(0);
        break;
    default:
        break;
    }
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
                        enemyLV1(70, k*3*2, 30, k, k % 2, tmp, enemy_frame);
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
                read(playerpipe[0],plarr,7*sizeof(int));
                //helper ci fornisce una specie di angolo per sparare diagonalmente i proietili
                helper = plarr[6];
                i = 0;

                //lettura pipes per mettere tutto in un array
                while (i < maxenemies){
                    read(tmp[0], arr[i], 7 * sizeof(int));
                    ++i;
                }
                //lettura per rimbalzi
                
                i = 0;
                w = 0;
                // debug
                /*
                mvwprintw(w1,1,26,"id:%d X:%d Y:%d ",arr[0][5],arr[0][0],arr[0][1]);
                mvwprintw(w1,2,26,"id:%d X:%d Y:%d ",arr[1][5],arr[1][0],arr[1][1]);
                //*/
                while (i < maxenemies)
                {   
                    //pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                    w = i+1;
                    while (w < maxenemies)
                    {   // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
                        // condizione molto lunga
                        
                        // controlo distanza tra i due nemici                  controllo se sono nella stessa x e che abbiano direzione diversa
                        if((abs(abs(arr[i][1]) - abs(arr[w][1])) < jumpbox) && arr[i][0] == arr[w][0] && arr[i][6] != arr[w][6])
                        {   //ulteriore controllo di direzione 
                            if ((arr[i][1] < arr[w][1] && arr[i][6] == 0 && arr[w][6] == 1) || (arr[i][1] > arr[w][1] && arr[i][6] == 1 && arr[w][6] == 0))
                            {
                                // salvataggio dei processi che necessitano di rimbalzo
                                jump[arr[w][5] + 1] = 1;
                                jump[arr[i][5] + 1] = 1;
                                // info di debug
                                //mvwprintw(w1, arr[i][5] + 3, 30, "Hit");
                                break;
                            }
                        }
                        ++w;
                    }
                    //collisione navetta/limite con nemico
                    if ((arr[i][0] <= 1) || (arr[i][1] == plarr[1] && arr[i][0] == plarr[0]) || life == 0){
                        jump[0] = 0;
                        player_started = 0;
                    }
                    //print nemici
                    printnemicolv1(arr[i][0], arr[i][1], w1);
                    mvwaddch(w1,arr[i][3], arr[i][2], '-');
                    //collisione navetta-proiettile_nemico
                    if (invincibility == 0){
                        if(arr[i][3] == plarr[1] && arr[i][2] == plarr[0]){
                            mvwaddch(w1,plarr[1], plarr[0], 'X');
                            --life;
                            invincibility = 120;
                        }
                    }

                    //collisione proiettile-navetta_nemica
                    if(plarr[2] == arr[i][0] && (plarr[3]-helper == arr[i][1] || plarr[3]+helper == arr[i][1])){
                        //magari si puo trovare un modo piu elegante per killare il processo nemico
                        kill(arr[i][4],SIGKILL);
                        mvwaddch(w1,arr[i][1], arr[i][0], 'X');
                        ++killed;
                    }
                    ++i;
                }
                //serve per ridurre i nemici nei vari counter
                while (killed)
                {
                    --maxenemies;
                    --killed;
                }
                //print a schermo
                mvwaddch(w1,plarr[3]+helper, plarr[2], 'o');
                mvwaddch(w1,plarr[3]-helper, plarr[2], 'o');
                invincibility = print_nave(invincibility,w1,plarr[0],plarr[1]);
                print_vita(life,w1);

                /* altre info di debug
                
                //printpl_info(w1,plarr);
                i = 0;
                while (i < ENEM_TEST )
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
                wrefresh(w1);
                i = 0;
                //sincronizzazione processi + invio info su rimbalzi
                while (i < maxenemies){
                    write(enemy_frame[1],jump,(ENEM_TEST + 1)*sizeof(int));
                    ++i;
                }
                i = 1;
                //reset dei rimbalzi
                while (i < ENEM_TEST + 1)
                {
                    jump[i] = 0;
                    ++i;
                }

            }
    }
    game_over(w1,plarr[0], plarr[1]);
    //chiusura lettore input
    kill(plarr[5],SIGKILL);
    i = 0;
    //chiusura pulita in caso di gameover per nemico che raggiunge il limite,necessita una revisione in modo che sia piu veloce
    while (i < maxenemies){
        read(tmp[0], arr[i], 7 * sizeof(int));
        ++i;
    }

}


/*
//collisione fine_schermo-nemici/nemico-navetta,gameover
                    
*/