#include "animations.h"
//velocita e impostazione animazione

//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori

/* game_over = Animazione di gameover.
 * int x prende le ascisse della navetta attuali(potrebbe non servire).
 * int y prende le ordinate della navetta attuali.
 * WINDOW w1, a quanto pare se non si passa questa funzione non viene visualizzato il carattere	*/
void game_over(WINDOW *w1,int x,int y){
	clear();
	refresh();
	start_color();
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
		mvprintw(y,x,NAVE_OK);
		bkgd(COLOR_PAIR(1));
		refresh();
		//Fine frame sfondo giallo
		napms(GM_SPEED);

		//Inizio frame sfondo rosso
		mvprintw(y,x,NAVE_ESPLOSA);
		bkgd(COLOR_PAIR(2));
		refresh();
		//Fine frame sfondo rosso
		napms(GM_SPEED);
	}
	
	//Inizio animazione morte lenta - Rosso chiaro
	napms(GM_DEATH);
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(3));
	refresh();
	napms(GM_DEATH-GM_SPEED);
     //Fine

	//Inizio animazione morte lenta - Rosso scuro
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(4));
	refresh();
	napms(GM_DEATH-GM_SPEED);
    //Fine

	//Inizio animazione morte lenta - Nero
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(5));
	refresh();
	napms(7*GM_SPEED);
	clear(); 
	refresh(); //Fine

	sleep(3);	

	/* Stampa lettera per lettera della stringa GAME OVER.
	 * mvprintw accepts only strings, so i need to convert 
	 * single char into strings to make the animation */
	for(i=0;i < GM_CYCLES2-1;i++){
		mvaddch(LINES/2,(COLS/2+i)-5,string[i]);
		refresh();
		napms(3*GM_SPEED);
        clear();
	}
	clear(); 
	refresh();
	napms(3*GM_SPEED);

	//Stampa della stringa GAME OVER per intero
	mvprintw(LINES/2,(COLS/2)-5,string);
	refresh();
	sleep(3);
    
    //Fine dell'esecuzione
	clear(); 
	refresh();
	endwin();
    exit(0);
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
}

int enemylinearbullet(int x,int y,int slownes, int pipe[]){
    int i = 0;
    while (x - i > -2){
        mvprintw(y,x-i,"-");
        mvprintw(y,x-i+1," ");
        //if(y == cordinata_nave(y) && x+i == cordinata_nave(x)) then nave colpita/distrutta
        refresh();
        napms(slownes);
        i++;
   
    }    
}
*/
//test nemici a livello 1,

# define pos_X 0
# define pos_Y 1

//l'ultimo paramtro e una pipe
int enemyLV1(int x,int y,int slownes,int *sender){
    //dichiarazioni iniziali
 
    int alive = 1;
    int enemy_pos[2] = {x,y}; //[0] = x,[1] = y;
    int send_info[4] = {};
    int bullet_Y = 0;
    int tmp = 0;
    int go_up = 1;
    pid_t enemy,bullet;
    int p_enemy_pos[2],p_bullet[2];
    pipe(p_enemy_pos);
    pipe(p_bullet);
    write(p_bullet[1], &tmp, sizeof(enemy_pos[pos_X]));
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

                write(sender[1],send_info,4*sizeof(int));
                napms(slownes);


                //ottenimento info per lanciare il processo proiettile
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
    close(p_enemy_pos[0]);
    close(p_enemy_pos[1]);
    close(p_bullet[0]);
    close(p_bullet[1]);
    exit(0);
}


//VERSIONE MOLTO INSTABILE,cambia la define per modificare il numero di nemici a schermo
int c;
int plarr[5] = {1,1,0,0,0};
int drawing = 0;
void *getinput(){
    int c;
    while (1){
        while(drawing){0+0;}
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
            default:
                break;
            }
        }
    }
}


#define ENEM_TEST 10
void screen(WINDOW *w1){
    pid_t proc,spawn,player;
    pthread_t inpt_id;
    //int tmp[ENEM_TEST][2];
    int tmp[2];
    int enemy_counter[2];
    int playerpipe[2];
    int arr[4];
 
    pipe(tmp);
    //pipe(tmp[1]);
    //pipe(tmp[2]);
    pipe(enemy_counter);
    pipe(playerpipe);
    int maxenemies = ENEM_TEST;
    write(enemy_counter[1],&maxenemies,sizeof(int));
    int i = 0;
    int j = -1;
    int player_started = 0;
    int c;
    int k = 0;
    //farlo con i processi distrugge la performance,conviene chiederlo al prof se possiamo usare i thread per entrambe le versioni
    pthread_create(&inpt_id, NULL, getinput, NULL);
    proc = fork();
    switch (proc)
    {
    //spawn
    case 0:
        while (k < maxenemies)
        {
            spawn = fork();
            switch (spawn)
            {
            case 0:        //variabili di spawn
                enemyLV1(70, 20 + (2 * k), 30, tmp);
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
        wait((int *)0);
        while (1)
        {  
            drawing = 0;
            refresh();
            napms(30);
            drawing = 1;
            clear();
            i = 0;
            while (i < maxenemies)
            {
                read(tmp[0], arr, 4 * sizeof(int));
                if (arr[0] <= 1)
                {
                    game_over(w1,plarr[0], plarr[1]);
                }
                mvaddch(arr[1], arr[0], '<');
                mvaddch(arr[3], arr[2], '-');
                ++i;
            }
            mvprintw(10, 10, "y=%d,x=%d", plarr[1], plarr[0]);
            mvaddch(plarr[1], plarr[0], '>');
        }
    }
}
