main: main.o utils.o poker.o game.o  hand.o exchange.o player.o montecarlo.o deck.o  card.o test.o
	gcc -o main main.o utils.o poker.o game.o  hand.o exchange.o player.o montecarlo.o deck.o  card.o test.o

main.o: main.c poker.h test.h
	gcc -c main.c 

test.o: test.c test.h
	gcc -c test.c

poker.o: poker.c poker.h
	gcc -c poker.c

deck.o: deck.c deck.h
	gcc -c deck.c

card.o: card.c card.h
	gcc -c card.c

utils.o: utils.c utils.h
	gcc -c utils.c

hand.o: hand.c hand.h
	gcc -c hand.c

exchange.o: exchange.c exchange.h
	gcc -c exchange.c

montecarlo.o: montecarlo.c montecarlo.h
	gcc -c montecarlo.c	

player.o: player.c player.h
	gcc -c player.c

game.o: game.c game.h
	gcc -c game.c

clean:
	\rm -f *.o main *~

