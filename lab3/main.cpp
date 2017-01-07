#include <tuple>
#include <iostream>
#include <fstream>

#include "TuplePrinter.h"
#include "CSVParser.h"

using namespace std;


int main()
{
  CSVParser<int, string, int> parser("input.txt");

//  auto row = parser.getRow();
//  cout << row << endl;
//
//  row = parser.getRow();
//  cout << row << endl;

//  CSVIterator<int, string, int> it = parser.begin();

//  for (; it != parser.end(); it++) {
//    cout << *it << endl;
//  }

  for (const auto& row : parser) {
    cout << row << endl;
  }

};