build: main.o animations.o 
	gcc main.o animations.o -lcurses -o exec.out
main.o: main.c
	gcc -c main.c
animations.o: ./assets/animations.c ./assets/animations.h
	gcc -c ./assets/animations.c  -o animations.o
clean:
	rm -rf ./*.o
