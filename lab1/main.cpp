#include <iostream>
#include <list>

#include "TritSet.hpp"

#define N 15

using namespace std;
using namespace alexgm;

int main(int argc, char const *argv[]) {

  TritSet set(N);

  set[3] = set[19] = True;
  set[7] = False;
  // set[19] = True;

  for (int i = 0; i < 22; i++)
    std::cout << i << ": " << set[i] << std::endl;

  return 0;
}


/*
 TODO:
 1) init TritSet | DONE
 2) [], = overload | DONE
 3) & overload

*/
