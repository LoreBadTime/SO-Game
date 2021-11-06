game: exec.out 
	echo "#!/bin/bash" > game.sh && echo "export TERM=xterm-256color" >> game.sh && echo "./exec.out" >> game.sh 
	chmod +x ./game.sh
exec.out: main.o animations.o 
	gcc main.o animations.o -o exec.out -lncurses
main.o: main.c
	gcc -c main.c 

animations.o: ./assets/animations.c ./assets/animations.h
	gcc -c ./assets/animations.c  -o animations.o
clean:
	rm -rf ./*.o
	rm -rf ./*.out
	rm -rf ./*.sh
