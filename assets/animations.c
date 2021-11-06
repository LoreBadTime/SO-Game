#include "animations.h"
//velocita e impostazione animazione

//NEL MAKEFILE INSERIRE QUESTA RIGA,OPPURE DIGITARE IL COMANDO NELLA SHELL:"export TERM=xterm-256color",abilitera la modifica e l'aggiunta di più colori


/* game_over = Animazione di gameover.
 * int x prende le ascisse della navetta attuali(potrebbe non servire).
 * int y prende le ordinate della navetta attuali.*/
void game_over(int x,int y){
	clear();
	refresh();
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
		clear();
		refresh();

		//Inizio frame sfondo rosso
		mvprintw(y,x,NAVE_ESPLOSA);
		bkgd(COLOR_PAIR(2));
		refresh();
		//Fine frame sfondo rosso
		napms(GM_SPEED);
		clear();
	}
	
	//Inizio animazione morte lenta - Rosso chiaro
	napms(GM_DEATH);
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(3));
	refresh();
	napms(GM_DEATH-GM_SPEED);
	clear();
	refresh(); //Fine

	//Inizio animazione morte lenta - Rosso scuro
	mvprintw(y,x,NAVE_ESPLOSA);
	bkgd(COLOR_PAIR(4));
	refresh();
	napms(GM_DEATH-GM_SPEED);
	clear();
	refresh(); //Fine

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
	for(i=0;i < GM_CYCLES2;i++){
		tmp[1] = string[i];
		mvprintw(LINES/2,(COLS/2+i)-5,tmp);
		refresh();
		napms(3*GM_SPEED);
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

int enemyLV1(int x,int y,int slownes){
    //dichiarazioni iniziali
    int alive = 1;
    int enemy_posX = x;
    int enemy_posY = y;
    int bullet_Y = 0;
    int tmp = 0;
    int go_up = 1;
    pid_t enemy,bullet;
    int p_enemy_X[2],p_enemy_Y[2],p_bullet[2];
    pipe(p_enemy_Y);
    pipe(p_enemy_X);
    pipe(p_bullet);
    write(p_bullet[1], &tmp, sizeof(enemy_posY));

    //ciclo che gesitisce il nemico
    while (enemy_posX > 0 && alive)
    {   
        //ciclo che gestisce il rimbalzo
        while ((enemy_posY >= 0 && enemy_posY <= 23))
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
                    if (go_up == 1){
                        enemy_posY--;
                    }
                    else{
                        enemy_posY++;
                    }

                    if (write(p_enemy_X[1], &enemy_posX, sizeof(enemy_posX)) <= 0){
                        mvprintw( 24, 15, "error writing enemy_posX");
                        refresh();
                    }
                    if (write(p_enemy_Y[1], &enemy_posY, sizeof(enemy_posY)) <= 0){
                        mvprintw( 25, 15, "error writing enemy_posY");
                        refresh();
                    }
                    exit(0);
                    break;
                }
            case -1:
                refresh();
                exit(-1);
                break;
            //processo schermo,molto probabilemente dovra essere modificato dato che per ora solo questa funzione lo gestisce
            //in futuro molto probabilmente sara costituito da pipe che riporteranno le cordinate
            default:
                if (read(p_enemy_X[0], &enemy_posX, sizeof(enemy_posX)) <= 0)
                {
                    mvprintw( 26, 15, "error reading enemy_posX outproc");
                    refresh();
                }
                if (read(p_enemy_Y[0], &enemy_posY, sizeof(enemy_posY)) <= 0)
                {
                    mvprintw( 27, 15, "error reading enemy_posY outproc");
                    refresh();
                }

                if(read(p_bullet[0], &tmp, sizeof(tmp)) <= 0){
                    mvprintw( 24, 15, "E reading tmp,while");
                    refresh();
                }
                mvprintw( enemy_posY, enemy_posX, "<");
                mvprintw( enemy_posY - 1, enemy_posX, " ");
                mvprintw( enemy_posY + 1, enemy_posX, " ");
                mvprintw( enemy_posY, enemy_posX - 1, " ");
                mvprintw( enemy_posY, enemy_posX + 1, " ");
                mvprintw( enemy_posY - 1, enemy_posX + 1, " ");
                mvprintw( enemy_posY + 1, enemy_posX + 1, " ");
                mvprintw( bullet_Y, tmp, "-");
                mvprintw( bullet_Y, tmp + 1, " ");
                //ottenimento info per lanciare il processo proiettile
                if(tmp <= -1){
                    tmp = enemy_posX;
                    bullet_Y = enemy_posY;
                }
                refresh();
                napms(slownes);
                break;
            }
        }
        //avanzamento fino alla navicella
        --enemy_posX;
        //cambio modalita di incremento delle y nella navicella(per il rimbalzo)
        go_up = !go_up;
        //incrementi per rientrare nel ciclo
        if (enemy_posY <= -1){
            enemy_posY++;
        }
        else{
            enemy_posY--;
        }
    }
    //questo nel caso in cui arrivi alla fine
    game_over(1,11);
}

