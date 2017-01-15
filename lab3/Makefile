CC=g++
LFLAGS= -Wall -Werror -g -std=c++11
CFLAGS= -c $(LFLAGS)


BIN_DIR=bin
BUILD_DIR=build
TEST_DIR=test
GTEST_DIR=../gtest

MAIN_OUT_NAME= CSVParser
TEST_OUT_NAME= test

HEADERS= CSVParser.h

MKDIR=mkdir -p


all: test

main:
	 $(MKDIR) $(BIN_DIR)
	 $(CC) $(LFLAGS) $(HEADERS) main.cpp -o $(BIN_DIR)/$(MAIN_OUT_NAME)

test: $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/test.o
	 $(CC) $(LFLAGS) -pthread $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/test.o -o $(TEST_DIR)/$(TEST_OUT_NAME)

$(BUILD_DIR)/test.o: $(TEST_DIR)/test.cpp
	$(CC) $(CFLAGS) -pthread -isystem $(GTEST_DIR)/include $(TEST_DIR)/test.cpp -o $(BUILD_DIR)/test.o

$(BUILD_DIR)/gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -pthread -isystem $(GTEST_DIR)/include -I $(GTEST_DIR) $(GTEST_DIR)/src/gtest-all.cc -o $(BUILD_DIR)/gtest-all.o

clean:
	 rm -rfdv *.o
