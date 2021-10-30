build: main.o example2.o example1.o
	gcc main.o example2.o example1.o -o exec.out
main.o: main.c
	gcc -c main.c
example2.o: example2.c example2.h 
	gcc -c example2.c
example1.o: example1.c example1.h
	gcc -c example1.c

clean:
	rm -rf ./*.o
