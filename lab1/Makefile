CC=g++
CFLAGS=-c -Wall -std=c++11

all: main clean

main: main.o TritSet.o
	$(CC) main.o TritSet.o -o main

main.o:
	$(CC) $(CFLAGS) main.cpp

TritSet.o:
	$(CC) $(CFLAGS) TritSet.cpp

clean:
	rm -rf *.o

cleanAll:
	rm -rf *.o main
