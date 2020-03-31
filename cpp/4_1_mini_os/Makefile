
main: main.o cv_stdlib.o statu.o core.o head.o
	gcc main.o cv_stdlib.o statu.o core.o head.o -o main
 
main.o: main.c main.h
	gcc -c main.c

cv_stdlib.o: cv_stdlib.c cv_stdlib.h
	gcc -c cv_stdlib.c

statu.o : statu.c statu.h
	gcc -c statu.c

head.o : head.c head.h
	gcc -c head.c

core.o : core.c core.h
	gcc -c core.c

