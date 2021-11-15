#include "animations.h"
//velocita e impostazione animazione

//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori

/* game_over = Animazione di gameover.
 * int x prende le ascisse della navetta attuali(potrebbe non servire).
 * int y prende le ordinate della navetta attuali.*/
void game_over(WINDOW *w1,int x,int y){
	wclear(w1);
	wrefresh(w1);
	//print sprite with x and y
	
	const char string[] = "GAME OVER";
	char tmp[2] = {' ', '\0'};
	int i = 0;

	//Definizione colori
	init_color(COLOR_BLACK,0, 0, 0);
	init_color(GM_LESS_DARKER_RED,125, 0, 0);
	init_color(GM_MORE_DARKER_RED,46, 9, 8);
	//Definizione coppie di colori
	init_pair(1,COLOR_RED,COLOR_YELLOW);
	init_pair(2,COLOR_WHITE,COLOR_RED);
	init_pair(3,COLOR_WHITE,GM_LESS_DARKER_RED);
	init_pair(4,COLOR_WHITE,GM_MORE_DARKER_RED);
	init_pair(5,COLOR_WHITE,COLOR_BLACK);

	//Animazione schermo che flasha
	for(i=0;i < GM_CYCLES;i++){
		//Inizio frame sfondo giallo
		mvwprintw(w1,y,x,NAVE_OK);
		wbkgd(w1,COLOR_PAIR(1));
        refresh();
		wrefresh(w1);
		//Fine frame sfondo giallo
		napms(GM_SPEED);

		//Inizio frame sfondo rosso
		mvwprintw(w1,y,x,NAVE_ESPLOSA);
		wbkgd(w1,COLOR_PAIR(2));
        refresh();
		wrefresh(w1);
		//Fine frame sfondo rosso
		napms(GM_SPEED);
	}
	
	//Inizio animazione morte lenta - Rosso chiaro
	napms(GM_DEATH);
	mvwprintw(w1,y,x,NAVE_ESPLOSA);
	wbkgd(w1,COLOR_PAIR(3));
    refresh();
	wrefresh(w1);
	napms(GM_DEATH-GM_SPEED);
     //Fine

	//Inizio animazione morte lenta - Rosso scuro
	mvwprintw(w1,y,x,NAVE_ESPLOSA);
	wbkgd(w1,COLOR_PAIR(4));
    refresh();
	wrefresh(w1);
	napms(GM_DEATH-GM_SPEED);
    //Fine

	//Inizio animazione morte lenta - Nero
	mvwprintw(w1,y,x,NAVE_ESPLOSA);
	wbkgd(w1,COLOR_PAIR(5));
    refresh();
	wrefresh(w1);
	napms(7*GM_SPEED);
	wclear(w1); 
    refresh();
	wrefresh(w1); //Fine

	sleep(3);	

	/* Stampa lettera per lettera della stringa GAME OVER.
	 * mvprintw accepts only strings, so i need to convert 
	 * single char into strings to make the animation */
	for(i=0;i < GM_CYCLES2-1;i++){
		mvwaddch(w1,LINES/2,(COLS/2+i)-5,string[i]);
		wrefresh(w1);
		napms(3*GM_SPEED);
        wclear(w1);
	}
	wclear(w1); 
	wrefresh(w1);
	napms(3*GM_SPEED);

	//Stampa della stringa GAME OVER per intero
	mvwprintw(w1,LINES/2,(COLS/2)-5,string);
	wrefresh(w1);
	sleep(3);
    
    //Fine dell'esecuzione
	wclear(w1); 
	wrefresh(w1);
}

//per adesso queste non servono,sono solo un template per l'algoritmo
/*
int linearbullet(int x,int y,int slownes){
    int i = 0;
    while (i < COLS + 1){
        mvprintw(y,x+i,"-");
        mvprintw(y,x+i-1," ");
        //if(y == cordinata_enemy(y) && x+i == cordinata_enemy(x)) then nemico colpito/distrutto
        refresh();
        napms(slownes);
        i++;
    }    
}*/


//test nemici a livello 1,

# define pos_X 0
# define pos_Y 1

//l'ultimo paramtro e una pipe
int enemyLV1(int x,int y,int slownes,int *sender,int *receiver){
    //dichiarazioni iniziali
 
    int alive = 1;
    int enemy_pos[2] = {x,y}; //[0] = x,[1] = y;
    int send_info[5] = {};
    int bullet_Y = 0;
    int tmp = 0;
    int rec = 0;
    int go_up = 1;
    pid_t enemy,bullet;
    int p_enemy_pos[2],p_bullet[2];
    pipe(p_enemy_pos);
    pipe(p_bullet);
    write(p_bullet[1], &tmp, sizeof(enemy_pos[pos_X]));
    close(receiver[1]);
    //ciclo che gesitisce il nemico
    while (enemy_pos[pos_X] > 0 && alive)
    {   
        //ciclo che gestisce il rimbalzo
        while ((enemy_pos[pos_Y] >= 0 && enemy_pos[pos_Y] <= 23)) //necessario fix per ottenere dinamicamente le y dello schermo
        {
            //creazione processo (P)schermo - (C)navicella/proiettile
            enemy = fork();
            switch (enemy){
            case 0:

                //creazione sottoprocessi (P)navicella (C)proiettile
                bullet = fork();
                switch (bullet)
                {

                //processo proiettile,serve per aquisire separatamente le coordinate del proiettile
                case 0:
                    close(p_bullet[0]);
                    --tmp;
                    if(write(p_bullet[1], &tmp, sizeof(tmp)) <= 0){
                        mvprintw( 24, 15, "E wr tmp,switch");
                        refresh();
                    }
                    close(p_bullet[1]);
                    exit(0);
                    break;
                //processo navicella,gestisce il rimbalzo eil movimento della navicella,potrebbe essere ottimizzabile
                default:
                    close(p_enemy_pos[0]);
                    if (go_up == 1){
                        enemy_pos[pos_Y]--;
                    }
                    else{
                        enemy_pos[pos_Y]++;
                    }
                    if (write(p_enemy_pos[1], enemy_pos, sizeof(int) * 2) <= 0){
                        mvprintw( 24, 15, "error writing enemy_pos[pos_X]-Y");
                        refresh();
                    }
                    close(p_enemy_pos[1]);
                    exit(0);
                    break;
                }
            case -1:
                refresh();
                exit(-1);
                break;
                //invio info del processo
           default:
                if (read(p_enemy_pos[0], enemy_pos, sizeof(int)*2) <= 0)
                {
                    mvprintw( 26, 15, "error reading enemy_pos[pos_X] outproc");
                    refresh();
                }
                if(read(p_bullet[0], &tmp, sizeof(tmp)) <= 0){
                    mvprintw( 24, 15, "E reading tmp,while");
                    refresh();
                }
                send_info[0] = enemy_pos[pos_X];
                send_info[1] = enemy_pos[pos_Y];
                send_info[2] = tmp;
                send_info[3] = bullet_Y;
                send_info[4] = getpid();

                write(sender[1],send_info,5*sizeof(int));
                read(receiver[0],&rec,sizeof(int));
                if (rec == 0){
                    enemy_pos[pos_Y] = -1;
                    alive = 0;
                }
                
                napms(slownes);


                //ottenimento info per lanciare il processo proiettile,se lo facciamo comunicare possiamo decidere il numero di proiettili generati
                if(tmp <= -1){
                    tmp = enemy_pos[pos_X];
                    bullet_Y = enemy_pos[pos_Y];
                }
                break;
            }
        }
        //avanzamento fino alla navicella
        --enemy_pos[pos_X];
        //cambio modalita di incremento delle y nella navicella(per il rimbalzo)
        go_up = !go_up;
        //incrementi per rientrare nel ciclo
        if (enemy_pos[pos_Y] <= -1){
            enemy_pos[pos_Y]++;
        }
        else{
            enemy_pos[pos_Y]--;
        }
    }
    //questo nel caso in cui arrivi alla fine
    send_info[0] = -1;
    send_info[1] = -1;
    send_info[2] = -1;
    send_info[3] = -1;
    send_info[4] = getpid();
    write(sender[1],send_info,5*sizeof(int));
    close(p_enemy_pos[0]);
    close(p_enemy_pos[1]);
    close(p_bullet[0]);
    close(p_bullet[1]);
    close(sender[1]);
    close(sender[1]);
    exit(0);
}


//VERSIONE MOLTO INSTABILE,cambia la define per modificare il numero di nemici a schermo
//queste variabili sono messe qui come globali solo per comodita,in futuro e meglio metterle nella funzione
int c;
int plarr[5] = {1,1,0,0,0};
int out = 1;

void *getinput(){
    int c;
    while (out){
        //meglio usare la getch,cosi non fa refresh allo schermo princiale
        c = getch();
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
            case KEY_RIGHT:
                plarr[0] += 1;
                break;
            case KEY_LEFT:
                plarr[0] -= 1;
                break;
            case (int)' ':
                plarr[4] = 1;
            default:
                break;
            }
        }
    }
}


#define ENEM_TEST 5
void screen(WINDOW *w1){
    pid_t proc,spawn,player;
    // purtroppo questa cosa deve essere asincrona,altrimenti la performance crolla completamente,per questo ho usato i thread qui per ottenere gli input
    // servirebbe chiedere ai prof che fare
    pthread_t inpt_id;
    int tmp[2];
    int enemy_frame[2];
    int playerpipe[2];
    int arr[5];
    int send = 1;
    out = 1;
    pipe(tmp);
    pipe(enemy_frame);
    pipe(playerpipe);
    int maxenemies = ENEM_TEST;
    int i = 0;
    int j = -1;
    int player_started = 1;
    int c;
    int k = 0;
    // purtoppo per adesso 6,dato che ad ogni fps viene contata la posizione,come workaround ho usato 6 per il numevo di vite
    // un fix con invincibilta e possiblie 
    int life = 6;
    //creazione thread
    pthread_create(&inpt_id, NULL, getinput, NULL);
    wclear(w1);
    proc = fork();
    //screazione processo schermo-nemici
    switch (proc)
    {
    //spawn dei nemici,diveneranno tutti proccessi separati
    case 0:   //k mi serve per decidere quando spawnarli
        while (k < maxenemies)
        {
            spawn = fork();
            switch (spawn)
            {
            case 0:        //variabili di spawn
                enemyLV1(70, 20 + (2 * k), 50, tmp, enemy_frame);
                //se si chiudono bene non passano da questo exit,di norma si auto terminano appena raggiungono la fine dello schermo
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
            wclear(w1);
            i = 0;
            mvwaddch(w1,plarr[1], plarr[0], '>');
            //questo non penso possa essere parallelizzabile,dato che deve mettere tutto nello schermo,aggiungere troppe pipe poi lo rallenterebbe
            if(plarr[4] == 1 && plarr[2] < plarr[0]){
                plarr[3] = plarr[1];
                plarr[2] = plarr[0];
                ++plarr[2];
            }
            if (plarr[2] > 1 && plarr[4] == 1)
            {
                ++plarr[2];
            }
            if(plarr[2] > 83 && plarr[4] == 1){
                plarr[4] = 0;
                plarr[2] = 0;
                plarr[3] = 0;
            }
            mvwaddch(w1,plarr[3], plarr[2], '-');
            while (i < maxenemies){
                read(tmp[0], arr, 5 * sizeof(int));
                //collisione fine_schermo-nemici/nemico-navetta,gameover
                if ((arr[0] <= 1) || (arr[1] == plarr[1] && arr[0] == plarr[0]) || life == 0){   
                    send = 0;
                    player_started = 0;
                    out = 0;
                }         
                mvwaddch(w1,arr[1], arr[0], '<');
                mvwaddch(w1,arr[3], arr[2], '-');
                //collisione navetta-proiettile_nemico
                if (arr[3] == plarr[1] && arr[2] == plarr[0]){
                    mvwaddch(w1,plarr[1], plarr[0], 'X');
                    --life;
                }

                //collisione processo navetta
                if(plarr[2] == arr[0] && plarr[3] == arr[1]){
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

            //mandando a 0 send invio a tutti i processi l'autokill,di norma pero serve per sincronizzare i processi con lo schermo(in modo da evitare lo sfarfallio di posizioni)
            while (i < maxenemies){
                write(enemy_frame[1],&send,sizeof(int));
                ++i;
            }
            
        }
    }
    game_over(w1,plarr[0], plarr[1]);
    i = 0;
    //chiusura pulita in caso di gameover
    while (i < maxenemies){
        read(tmp[0], arr, 5 * sizeof(int));
        ++i;
    }

}

