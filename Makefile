main: poker.o main.o test.o
	gcc -o main main.o poker.o test.o

main.o: main.c poker.h test.h
	gcc -c main.c 

test.o: test.c test.h
	gcc -c test.c

poker.o: poker.c poker.h
	gcc -c poker.c

clean:
	\rm -f *.o main *~

