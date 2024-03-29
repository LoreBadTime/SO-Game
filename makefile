game: exec.out 
	echo "#!/bin/bash" > game.sh && echo "export TERM=xterm-256color" >> game.sh && echo "./exec.out" >> game.sh 
	chmod +x ./game.sh
exec.out: main.o animations.o sprites.o threads.o process.o
	gcc main.o animations.o sprites.o threads.o process.o -o exec.out -lncurses -pthread -lpthread -lrt
main.o: main.c
	gcc -c main.c 
process.o: ./versione_processi/process.c ./versione_processi/process.h
	gcc -c ./versione_processi/process.c  -o process.o
threads.o: ./versione_threads/threads.c ./versione_threads/threads.h
	gcc -c ./versione_threads/threads.c  -o threads.o
sprites.o: ./assets/sprites.c ./assets/sprites.h
	gcc -c ./assets/sprites.c  -o sprites.o
animations.o: ./assets/animations.c ./assets/animations.h
	gcc -c ./assets/animations.c  -o animations.o
clean:
	rm -rf ./*.o
	rm -rf ./*.out
	rm -rf ./*.sh
