project(src)

list(APPEND SOURCE_LIBS TritSet.cpp)
list(APPEND HEADERS TritSet.h TritSetIterator.h)

add_library(src STATIC ${SOURCE_LIBS} ${HEADERS})