/*#include "../globalincludes.h"
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int sync = 1;
//implementata mediante thread,necessita reimplementazione processi
void *getinput(void *data){
    int *plarr = (int *)data;
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

typedef struct {
  int x;
  int y;
  int id;
  int direzione;
  int *send_info;
  int *rec;
} send_to_thread;

int thread_enemy(void *data){
    // lock delle variabili perche non impazzisca lo spawner
    pthread_mutex_lock(&mutex);
    send_to_thread* real_data = (send_to_thread *) data;
    //Inizializzazione nemico,info del nemico
    Navetta_Nemica nemico;
    nemico.proiettile.x=0;
    nemico.proiettile.y=0;
    nemico.proiettile.ready=0;
    nemico.navnemica.x=real_data->x;
    nemico.navnemica.y=real_data->y;
    int id = real_data->id;
    int direzione = real_data->direzione;
    int *send_info = real_data->send_info;
    int *rec = real_data->rec;
    pthread_mutex_unlock(&mutex);
    int firstcycle = 0;
    int alive = 1; //Stato navicella nemica
    //Inizializzazione navicella nemica
    //int direzione = 1; //Direzione navicella nemica, 1 = Da alto in basso, 0 = Da basso in alto

    //Risoluzione e pipe per inviare le coordinate
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    //Ciclo che gesitisce il nemico
    while (nemico.navnemica.x > 0 && alive)
    {
        //Ciclo che gestisce il rimbalzo
        while ((nemico.navnemica.y >= 0 && nemico.navnemica.y <= maxy))
        {
                    --nemico.proiettile.x;
                    if (direzione)
                    {
                        nemico.navnemica.y--;
                    }
                    else
                    {
                        nemico.navnemica.y++;
                    }
                    
                    send_info[0] = nemico.navnemica.x;
                    send_info[1] = nemico.navnemica.y;
                    send_info[2] = nemico.proiettile.x;
                    send_info[3] = nemico.proiettile.y;
                    send_info[4] = 1;
                    send_info[5] = id;
                    send_info[6] = direzione;
                    //mettiamo il thread in wait
                    while (sync == 0 && firstcycle){;}
                    if(firstcycle == 0){
                        firstcycle = 1;
                    }
                    while (sync){;}
                    //killa navicella nemica
                    if (rec[0] == 0 || rec[id + 1] == -1){
                        nemico.navnemica.y = -1;
                        alive = 0;
                    }
                    //dall'array estrae il suo id,serve per il rimbalzo in caso di collisioni con le navette nemiche
                    if(rec[id + 1]){
                        direzione = !direzione;
                    }
                    rec[id + 1] = 0;
                    //Ottenimento info per lanciare il processo proiettile,+ randomizzazione lancio proiettile(altrimenti diventa un bullet hell)
                    if(nemico.proiettile.x <= -1 && (rand() % 101 == 0)){
                        nemico.proiettile.x = nemico.navnemica.x;
                        nemico.proiettile.y = nemico.navnemica.y;
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
    send_info[0] = 10;
    send_info[1] = -30;
    send_info[2] = -1;
    send_info[3] = -1;
    send_info[4] = -1;
    send_info[5] = -1;
    send_info[6] = -1;
}
//plarr contiente tutte le info del player
//sys_slownes e la velocita di gioco
/*
void gestore_input(int *plarr,int *pipe,int sys_slownes){
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
        // ottenimento dati da keyboard
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
                while(c == getch()){;}         
        }
    write(pipe[1],plarr,7*sizeof(int));
    napms(sys_slownes);
    }
}
*/
// stampa info player(non necessaria)
/*
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
//schermo per processi

void screen_threads(WINDOW *w1){
    //array che contiene le info del proiettile
    //x,y,xproiettile,yproiettile,proiettile disponibile/lanciato,id processo input,modifica angolo proiettile
    int plarr[7] = {5,10,0,0,0,0,0};
    pid_t proc,spawn;
   //pthread_t inpt_id;
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
    // thread di input   
    pthread_t inpt_id;
    pthread_create(&inpt_id, NULL, getinput, (void*)plarr); 
    send_to_thread data;
    pthread_t enemies_id[ENEM_TEST];
    while (k < maxenemies)
    {
        data.x = 70;
        data.y =  k * 3 * 2;
        data.id = k;
        data.direzione = k%2;
        data.send_info = arr[k];
        data.rec = jump;
        // variabili di spawn nemici
        pthread_create(&(enemies_id[k]),NULL,thread_enemy,(void*) &data);

        ++k;
    }
            // finche non raggiungo il gameover,scrivo lo schermo
            while (player_started)
            {   
                wclear(w1);//prossimo frame,la clear in compenso all'erase non cancella lo schermo ma solo il buffer dello schermo
                //helper ci fornisce una specie di angolo per sparare diagonalmente i proietili
                helper = plarr[6];
                //pronti per la lettura
                i = 0;
                while (i < maxenemies){ 
                    //il thread non li posso realmente bloccare nello stesso modo come si faceva con i processi,
                    //per riciclare i vecchi algoritmi ho preferito lockarlo in questo modo
                    while (arr[i][4] == 0 && arr[i][4] != -1){;} 
                    arr[i][4] = 0;
                    ++i;
                }
                //possibile idea per velocizzazione
                
                //lettura per rimbalzi

                // debug
                /*
                mvwprintw(w1,1,26,"id:%d X:%d Y:%d ",arr[0][5],arr[0][0],arr[0][1]);
                mvwprintw(w1,2,26,"id:%d X:%d Y:%d ",arr[1][5],arr[1][0],arr[1][1]);
                //
                i = 0;
                w = 0;
                //controllo collisioni
                while (i < maxenemies)
                {   
                    ///*pseudo selectionsort per controllare il rimbalzo in caso di collisioni
                    w = i+1;
                    while (w < maxenemies)
                    {   // modificando jumpbox si puo modificare il rilevamento di caselle prima di fare il salto
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
                        ++w;
                    }
                    //
                    //collisione navetta/limite con nemico
                    if ((arr[i][0] <= 1) || (arr[i][1] == plarr[1] && arr[i][0] == plarr[0]) || life == 0){
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
                        if(arr[i][3] == plarr[1] && arr[i][2] == plarr[0]){
                            mvwaddch(w1,plarr[1], plarr[0], 'X');
                            --life;
                            invincibility = 120;
                        }
                    }

                    //collisione proiettile-navetta_nemica //
                    if(plarr[2] == arr[i][0] && ((hitbox > plarr[3]-helper-arr[i][1] && -hitbox < plarr[3]-helper-arr[i][1]) || (-hitbox < plarr[3]+helper-arr[i][1] && hitbox > plarr[3]+helper-arr[i][1] ))){
                        //possiamo sfruttare questo meccanismo per spawnare nemici da lv 1 a lv 2,e in generale per togliere la vita ai nemici
                        jump[arr[i][5] + 1] = -1;
                        mvwaddch(w1,arr[i][1], arr[i][0], 'X');
                        //serve per ridurre i nemici nei vari counter
                        ++killed;
                    }
                    ++i;
                }
                //sync 1
                sync = 0;
                i = 1;
                while (i < maxenemies){ 
                    //il thread non li posso realmente bloccare nello stesso modo come per i processi,
                    //per riciclare i vecchi algoritmi ho preferito lockarlo in questo modo
                    while (jump[i] != 0){;} 
                    ++i;
                }
                //risincronizzazione dopo risorsa condivisa
                sync = 1;
                //stampa a schermo
                //stampa proiettili navetta
                mvwaddch(w1,plarr[3]+helper, plarr[2], 'o');
                mvwaddch(w1,plarr[3]-helper, plarr[2], 'o');
                //stampa navetta
                invincibility = print_nave(invincibility,w1,plarr[0],plarr[1]);
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
                }

                
                //riduzione cicli necessari generali,migliorabile(maxenemies = maxenemies - killed;)
                wrefresh(w1);
            }
    }

    game_over(w1,plarr[0], plarr[1]);
    //chiusura lettore input
    kill(plarr[5],SIGKILL);
    /*
    i = 0;
    //chiusura pulita in caso di gameover per nemico che raggiunge il limite,necessita una revisione in modo che sia piu veloce
    while (i < maxenemies){
        read(tmp[0], arr[i], 7 * sizeof(int));
        ++i;
    }
    */

}
*/