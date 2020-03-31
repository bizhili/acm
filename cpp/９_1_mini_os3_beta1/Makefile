main: main.o cv_stdlib.o 
	gcc main.o cv_stdlib.o  -o main
 
main.o: main.c main.h
	gcc -c main.c

cv_stdlib.o : cv_stdlib.c cv_stdlib.h
	gcc -c cv_stdlib.c