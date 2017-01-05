#include <tuple>
#include <iostream>

#include "TuplePrinter.h"

using namespace std;


int main()
{
  tuple<int, double, int, string> t{1, 2.4, 15, "hello"};

  cout << t;
}