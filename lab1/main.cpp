#include <iostream>
#include <cassert>
//
#include "src/TritSet.h"
//
using namespace std;
using namespace alexgm;

int main(int argc, char const *argv[]) {


#define ENABLE_TESTS
#ifdef ENABLE_TESTS

  TritSet set(1000);
  size_t allocLength = set.capacity();

  set[1000000000] = Unknown;

  if(set[2000000000] == True){}

  set[1000000000] = True;


  allocLength = set.capacity();
  set[1000000000] = Unknown;
  set[1000000] = False;


  set.shrink();

  TritSet setA(3);
  TritSet setB(500);
  setA[0] = setA[2] = True;

  setB[1] = False;
  setB[2] = True;

  TritSet setC = (setB & setA) | setB;
#endif

  const TritSet setD(10);

  setD[0] = setD[4] = setD[5] = False;
  setD[3] = setD[29] = True;

  cout << "length: " << setD.length() << endl;

//  for (auto it = setD.begin(); it != setD.end(); ++it) {
//    cout << (*it == False) << endl;
//    *it = False;
//  }

  for (TritSet::const_iterator it = setD.begin(); it != setD.end(); ++it) {
    cout << (*it == True) << endl;
//    it = True;
  }

  auto card = setD.cardinality();
  cout << "False: " << card[False] << endl;
  cout << "Unknown: " << card[Unknown] << endl;
  cout << "True: " << card[True] << endl;




  return 0;
}
