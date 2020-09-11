CC = g++

all: main

main: main.o
	$(CC) -g -Wall -o tp1Pdi main.o -lm -lX11 -lpthread

main.o: main.cpp main.h
	$(CC) -g -Wall -c main.cpp -o main.o -lX11 -lpthread

clean:
	rm -f *.o tp1Pdi
