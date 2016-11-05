# Compiler options

CC=g++
CFLAGS=-c -Wall -g -std=c++11
LFLAGS= -Wall -g -std=c++11

# Paths settings

GTEST_DIR=tests/lib/g-test
MY_TEST_PATH=tests/basic_tests
SRC_DIR=src
LIB_DIR=lib
BIN_DIR=build

# Shell commands

MK_DIR=mkdir -p

# Sources

STATIC_LIB_OBJECTS=TritSet.o
STATIC_LIB_FINAL_OBJECT=libtritset.o
STATIC_LIB_HEADERS=$(SRC_DIR)/TritSet.h $(SRC_DIR)/TritSetIterator.h
STATIC_LIB_NAME=libtritset.a

MAIN_OUTPUT_NAME=lab1


all: buildStaticLib main buildTest

release: all clean

main: main.o TritSet.o
	$(MK_DIR) $(BIN_DIR)
	$(CC) main.o TritSet.o -o $(BIN_DIR)/$(MAIN_OUTPUT_NAME)

main.o:
	$(CC) $(CFLAGS) main.cpp

TritSet.o:
	$(CC) $(CFLAGS) $(SRC_DIR)/TritSet.cpp

buildTest: buildTest_ clean

buildTest_: buildGTest TritSet.o
	$(CC) $(LFLAGS) -isystem $(GTEST_DIR)/include -pthread $(MY_TEST_PATH)/tritset_check.cpp TritSet.o libgtest.a \
	    -o $(BIN_DIR)/tritset_test

buildGTest:
	$(CC) $(CFLAGS) -isystem $(GTEST_DIR)/include -I $(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

buildStaticLib: $(STATIC_LIB_OBJECTS)
	$(MK_DIR) $(LIB_DIR)
	ar -rvs $(LIB_DIR)/$(STATIC_LIB_NAME) $(STATIC_LIB_OBJECTS)

clean:
	rm -rf *.o ./*.a