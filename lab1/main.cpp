#include <iostream>
#include <list>

#include "TritSet.hpp"

#define N 15

using namespace std;
using namespace alexgm;

int main(int argc, char const *argv[]) {

  TritSet set1(3);
  TritSet set2(3);
  TritSet set3(3);

  set1[0] = True;
  set1[2] = True;

  set2[0] = False;
  set2[1] = True;
  set2[2] = True;

  set1 = set1 & set2 & set3;

  for (size_t i = 0; i < set1.length(); i++) {

    bool b = (set1[i] == set2[i]);
    std::cout << "value: " << set1[i] << std::endl;
  }

  return 0;
}


/*
 TODO:
 1) init TritSet | DONE
 2) [], = overload | DONE
 3) & | ~ overload

*/
