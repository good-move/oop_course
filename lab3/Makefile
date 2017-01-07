CC=g++
LFLAGS= -Wall -Werror -g -std=c++11
CFLAGS= -c $(LFLAGS)


BIN_DIR=bin
BUILD_DIR=build
TEST_DIR=test

OUT_NAME= CSVParser.out

HEADERS= CSVParser.h CSVIterator.h


all: main

main:
	 $(CC) $(LFLAGS) $(HEADERS) main.cpp -o $(BIN_DIR)/$(OUT_NAME)

clean:
	 rm *.o
