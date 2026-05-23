.PHONY: clean

all: main

main: src/main.c libgame.a 
	gcc src/main.c -I./include -L. -lgame -o program
	
game.o: src/game.c include/game.h
	gcc -c src/game.c -I./include -o game.o

libgame.a: game.o
	ar rcs libgame.a game.o
clean:
	rm -rf *.o main