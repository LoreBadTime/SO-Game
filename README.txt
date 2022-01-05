*****************************************************************************************************************************
*
*	  Space Defender - Progetto di:
*	
*   [60/61/65951] Lorenzo Vittorio Concas   - l.concas16@studenti.unica.it
*   [60/61/65922] Salvatore Andrea Castello - salvatoreandreacastello@gmail.com / s.castello@studenti.unica.it
*
* 	Il gioco presenta la versione processi e thread, separate dalle due cartelle con il proprio makefile.
*   Tutto il gioco è stato sviluppato sullo stesso Sistema Operativo e testato nella stessa Vm indicata dai docenti (64 bit).
*
*   Per muoversi all'interno del gioco è sufficiente premere i tasti freccia, invio per selezionare e spacebar per sparare
*	
*	  Il gioco presenta al suo interno una quantità di nemici modificabile nelle impostazioni del gioco stesso
*	  che vanno da 1 a 30 nemici. Il valore 30 è solo un valore "simbolico", infatti è possibile (ma non consigliato)
*	  overclockare il numero di nemici presenti facendo una piccola modifica al codice, risoluzione permettendo.
*	
* 	Il gioco non supporta il cambio di risoluzione a RUN-TIME, quindi una volta avviato si prega di mantenere
*	  la stessa risoluzione con cui è stato lanciato il programma.
*
*   Nonostante sia raro che capiti, onde evitare malfunzionamenti al gioco, se si è desidera avviare il gioco in una risoluzione
*   qualsiasi diversa rispetto alla precedente si DEVE riaprire un nuovo terminale PRIMA e DOPO averlo
*   terminato, in quanto le funzioni di ncurses creano problemi una volta che si passa/torna in una risoluzione maggiore/inferiore.
*
*	  Nonostante sia possibile terminare in qualsiasi momento l'esecuzione del gioco usando la combinazione di tasti "CTRL + C",
*   sconsigliamo vivamente di farlo, poichè questo potrebbe creare bug critici all'interno del gioco, e di cliccare direttamente
*   EXIT una volta che si desidera terminare il gioco.
*
*	  Attraverso le impostazioni è possibile scegliere il colore della nave principale, il numero di nemici e le vite.
*
*	  La versione processi presenta una write all'interno della funzione screen(), abbiamo trovato questa funzione
* 	necessaria per poter implementare i rimbalzi tra i nemici stessi. Oltre ai rimbalzi mandiamo alla funzione anche
*	  quali nemici devono essere uccisi, tutto nello stesso array.
* 	Abbiamo preferito utilizzare più pipe per rendere il gioco molto più fluido.
*
*   Per la versione thread abbiamo invece usato più semafori e un solo mutex.
*   Questo ci ha permesso di poter girare il gioco in VM con più di un processore.
*
*	  Cordiali saluti e speriamo che il nostro gioco vi piaccia!
*
*****************************************************************************************************************************
