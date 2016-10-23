cmake_minimum_required(VERSION 3.6)
project(15201_boltava)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lab1/main.cpp
    lab1/TritSet.cpp
    lab1/TritSet.hpp)

add_executable(15201_boltava ${SOURCE_FILES})