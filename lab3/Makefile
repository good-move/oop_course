CC=g++
LFLAGS= -Wall -Werror -g -std=c++11
CFLAGS= -c $(LFLAGS)

OUT_NAME= CSVParser

all:
	 $(CC) $(LFLAGS) CSVParser.h CSVIterator.h main.cpp -o $(OUT_NAME)

clean:
	 rm *.o
