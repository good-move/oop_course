#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

using std::stringstream;
using std::ifstream;
using std::string;
using std::vector;
using std::tuple;

template <class ...Args>
class CSCParser;

template <class PointerType, class ValueType>
class CSVParserIterator;


namespace {
  template <class ...Types> struct StringParser;

  template <>
  struct StringParser<> {
      static tuple<int> parse(size_t index, const vector<string>& items) {
        throw std::invalid_argument("No type provided to parse a string.");
      };
  };

  template <class T>
  struct StringParser<T> {
      static tuple<T> parse(size_t index, const vector<string>& items) {
        T t;
        stringstream ss(items.at(index));
        ss >> t;
        tuple<T> tuple(t);

        return tuple;
      };
  };

  template <class T, class ...Types>
  struct StringParser<T, Types...> {
      static tuple<T, Types...> parse(size_t index, const vector<string>& items) {
        // add check for ...Types size

        T t;
        stringstream ss(items.at(index));
        ss >> t;
        tuple<T> tuple(t);

        return std::tuple_cat(tuple, StringParser<Types...>::parse(index + 1, items));
      }
  };
}



template <class ...Args>
class CSVParser {
  public:
    using csv_parser_iterator = CSVParserIterator<CSVParser<Args...>*, tuple<Args...>>;

    CSVParser(string fname) : filename_(fname) {};

    csv_parser_iterator begin();
    csv_parser_iterator end();

    void split(std::istream&, const char, vector<string>&, size_t maxElements = 1);
    vector<string> getRow(std::istream &);
    tuple<Args...> parse(const vector<string>& items) {
      return StringParser<Args...>::parse(0, items);
    };

  private:
    string filename_;
    char colDelim_ = ',';
    char rowDelim_ = '\n';
    char screenChar_ = '"';
};


template <class ...Args>
vector<string>
CSVParser<Args...>::
getRow(std::istream &is)
{
  vector<string> rowVector;
  split(is, rowDelim_, rowVector);

  if (rowVector.size() == 1) {
    string row = rowVector[0];
    vector<string> rowElements;
    stringstream ss(row);
    split(ss, colDelim_, rowElements, row.length());
    return rowElements;
  }

  throw std::runtime_error("Couldn't read from input stream.");


//  MOVE THIS TO ANOTHER FUNCTION
//    if (rowElements.size() != sizeof...(Args)) {
//      throw std::invalid_argument("Invalid row length. Expected " + sizeof...(Args));
//    }

}

template <class ...Args>
void
CSVParser<Args...>::
split(std::istream& is, const char delimiter, vector<string> &elements, size_t maxElements)
{
  bool screeningIsOn = false;
  string element;
  char c;

  while (!is.eof() && maxElements > 0) {
    is.get(c);

    if (c == screenChar_) {
      screeningIsOn = true;
      continue;
    }

    if (!screeningIsOn && c == delimiter || is.eof()) {
      if (element.length() > 0) {
        elements.push_back(element);
        element = "";
        --maxElements;
      }
      continue;
    }

    element += c;
  }
}

template <class ...Args>
typename CSVParser<Args...>::csv_parser_iterator
CSVParser<Args...>::
begin()
{

}

template <class ...Args>
typename CSVParser<Args...>::csv_parser_iterator
CSVParser<Args...>::
end()
{

}
