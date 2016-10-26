cmake_minimum_required(VERSION 3.6)
project(15201_boltava)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")

set(SOURCE_FILES lab1/main.cpp)
add_executable(lab1_run ${SOURCE_FILES})

include_directories(lab1/src)

add_subdirectory(lab1/tests)
add_subdirectory(lab1/src)

target_link_libraries(lab1_run src)