#include <iostream>

#include "src/TritSet.h"

using namespace std;
using namespace alexgm;

void f(Trit t1, Trit t2) {
  cout << t1 << endl;
  cout << t2 << endl;
}

int main()
{

  TritSet B(10);
  B[3] = B[4] = B[8] = True;
  B[1] = B[6] = False;


  for (const auto it : B) {
    cout << it << endl;
  }

//  for (auto& it : B) {
//    it = Unknown;
//  }

  return 0;
}