# Compiler options

CC=g++
CFLAGS=-c -Wall -Werror -Wextra -g -std=c++11
LFLAGS= -Wall -Werror -g -std=c++11

# Paths settings

GTEST_DIR=../gtest
INC_DIR=include
TEST_DIR=test
SRC_DIR=src
LIB_DIR=lib
BIN_DIR=bin
BUILD_DIR=build

# Shell commands

BASH_MK_DIR=mkdir -p

# Sources

MAIN_OUTPUT_NAME=main


all: parser surface test main clean

buildAll: all

parser: anyoption.so
	$(CC) $(CFLAGS) $(SRC_DIR)/Parser.cpp -o parser.o

anyoption.so:
	$(CC) $(CFLAGS) -shared 3rd_party/anyoption/anyoption.cpp -o anyoption.so

surface: serializer buildSurface buildSimpleSurface dictionary

serializer:
	$(BASH_MK_DIR) build
	$(CC) $(CFLAGS) $(SRC_DIR)/serializer/SimpleSurfaceSerializer.cpp -o $(BUILD_DIR)/simple_surf_serialzer.o
	$(CC) $(CFLAGS) $(SRC_DIR)/serializer/DictionarySerializer.cpp -o $(BUILD_DIR)/dictionary_serialzer.o

buildSurface:
	$(CC) $(CFLAGS) $(SRC_DIR)/surface/SimpleSurface.cpp -o $(BUILD_DIR)/SimpleSurface.o

buildSimpleSurface: buildPlane buildCylinder buildTorus

buildPlane:
	$(BASH_MK_DIR) lib
	$(CC) $(CFLAGS) -shared $(SRC_DIR)/surface/Plane.cpp -o $(LIB_DIR)/plane_lib.so

buildCylinder:
	$(CC) $(CFLAGS) -shared $(SRC_DIR)/surface/Cylinder.cpp -o $(LIB_DIR)/cylinder_lib.so

buildTorus:
	$(CC) $(CFLAGS) -shared $(SRC_DIR)/surface/Torus.cpp -o $(LIB_DIR)/torus_lib.so

dictionary:
	$(CC) $(CFLAGS) -shared $(SRC_DIR)/surface/Dictionary.cpp -o $(LIB_DIR)/dictionary_lib.so

test: libgtest.a
	$(CC) $(LFLAGS) -pthread -isystem $(GTEST_DIR)/include -I $(GTEST_DIR) $(TEST_DIR)/test.cpp libgtest.a \
	     $(BUILD_DIR)/SimpleSurface.o $(LIB_DIR)/torus_lib.so $(LIB_DIR)/cylinder_lib.so $(LIB_DIR)/plane_lib.so \
	     $(BUILD_DIR)/simple_surf_serialzer.o $(BUILD_DIR)/dictionary_serialzer.o \
	     $(LIB_DIR)/dictionary_lib.so \
	    -o $(TEST_DIR)/test

libgtest.a:
	$(CC) $(CFLAGS) -isystem $(GTEST_DIR)/include -I $(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

main: main.o
	$(BASH_MK_DIR) $(BIN_DIR)
	$(CC) $(LFLAGS) main.o \
	     $(BUILD_DIR)/SimpleSurface.o $(LIB_DIR)/torus_lib.so $(LIB_DIR)/cylinder_lib.so $(LIB_DIR)/plane_lib.so \
	     $(BUILD_DIR)/simple_surf_serialzer.o $(BUILD_DIR)/dictionary_serialzer.o \
	     parser.o anyoption.so  $(LIB_DIR)/dictionary_lib.so \
	    -o $(BIN_DIR)/$(MAIN_OUTPUT_NAME)
main.o:
	$(CC) $(CFLAGS) main.cpp -o main.o

clean:
	rm ./*.a *.o *.so

cleanAll:
	rm -rf ./*.a *.o *.so
