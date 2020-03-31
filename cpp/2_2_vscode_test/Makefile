


main: main.o add.o
	gcc main.o add.o -o main
 
main.o: main.c add.h
	gcc -c main.c
 
add.o: add.c add.h
	gcc -c add.c