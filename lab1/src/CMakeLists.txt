project(src)

list(APPEND SOURCE_LIBS TritSet.cpp)

add_library(src STATIC ${SOURCE_LIBS})