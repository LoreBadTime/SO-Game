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
