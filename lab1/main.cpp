#include <iostream>
#include <cassert>

#include "src/TritSet.h"

using namespace std;
using namespace alexgm;

int main(int argc, char const *argv[]) {

#define ENABLE_TESTS
#ifdef ENABLE_TESTS

  TritSet set(1000);

  size_t allocLength = set.capacity();
  assert(allocLength >= 1000 * 2 / 8 / sizeof(uint));
  if (allocLength >= 1000 * 2 / 8 / sizeof(uint)) {
    std::cout << "success" << std::endl;
  }

  set[1000000000] = Unknown;
  assert(allocLength == set.capacity());
  if (allocLength == set.capacity()) {
    std::cout << "success" << std::endl;
  }

  if(set[2000000000]==True){}
  assert(allocLength == set.capacity());
  if (allocLength == set.capacity()) {
    std::cout << "success" << std::endl;
  }

  set[1000000000] = True;
  assert(allocLength < set.capacity());
  if (allocLength < set.capacity()) {
    std::cout << "success" << std::endl;
  }


  allocLength = set.capacity();
  set[1000000000] = Unknown;
  set[1000000] = False;
  assert(allocLength == set.capacity());
  if (allocLength == set.capacity()) {
    std::cout << "success" << std::endl;
  }

  set.shrink();
  assert(allocLength > set.capacity());
  if (allocLength > set.capacity()) {
    std::cout << "success" << std::endl;
  }

  TritSet setA(3);
  TritSet setB(500);
  setA[0] = setA[2] = True;

  setB[1] = False;
  setB[2] = True;

  TritSet setC = (setB & setA) | setB;
  assert(setC.capacity() == setB.capacity());

  if (setC.capacity() == setB.capacity()) {
    std::cout << "success" << std::endl;
  }

#endif

  TritSet setD(10);

  setD[0] = setD[4] = setD[5] = False;
  setD[3] = setD[29] = True;

  cout << "length: " << setD.length() << endl;

  for (auto it : setD) {
    cout << it << endl;
  }

  return 0;
}
