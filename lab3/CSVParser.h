#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using std::stringstream;
using std::ifstream;
using std::string;
using std::vector;
using std::tuple;

template <class ...Args>
class CSVParser;

template <class ...Args>
class CSVIterator;


namespace {

  template <class T>
  tuple<T> convert_to_tuple(const string& str) {
    T t;
    stringstream ss(str);
    ss >> t;
    if (ss.fail()) throw std::invalid_argument("Cannot parse string " + str);

    return tuple<T>{t};
  }

  template <class ...Types> struct RowParser;

  template <>
  struct RowParser<> {
      static tuple<int> parse(size_t index, const vector<string>& items) {
        throw std::invalid_argument("No type provided to parse a row.");
      };
  };

  template <class T>
  struct RowParser<T> {
      static tuple<T> parse(size_t index, const vector<string>& items) {
        try {
          return convert_to_tuple<T>(items.at(index));
        } catch (std::invalid_argument&){
          throw std::invalid_argument(std::to_string(index));
        }
      };
  };

  template <class T, class ...Types>
  struct RowParser<T, Types...> {
      static tuple<T, Types...> parse(size_t index, const vector<string>& items) {
        // add check for ...Types size
        tuple<T> tuple;
        try {
          tuple = convert_to_tuple<T>(items.at(index));
        } catch (std::invalid_argument&){
          throw std::invalid_argument(std::to_string(index));
        }
        return std::tuple_cat(tuple, RowParser<Types...>::parse(index + 1, items));
      }
  };
}


template <class ...Args>
class CSVParser{
  public:
    using csv_iterator = CSVIterator<Args...>;

    CSVParser(const string& fname);

    tuple<Args...> getRow();

    csv_iterator begin() { return csv_iterator(this, stream_.eof()); };
    csv_iterator end() { return csv_iterator(this, true); };

  private:
    void splitStream(std::istream&, const char, vector<string>&, size_t maxElements = 1);
    vector<string> splitLine(const string&);
    string readLine();

    tuple<Args...> parseLine(const vector<string> &items) {
      return RowParser<Args...>::parse(0, items);
    };

    std::ifstream stream_;
    char colDelim_ = ',';
    char rowDelim_ = '\n';
    char escapeChar_ = '\\';
};



template <class ...Args>
CSVParser<Args...>::
CSVParser(const string& fname)
{
  stream_.open(fname);

  if (stream_.fail()) throw std::runtime_error("Couldn't open file " + fname);
}

template <class ...Args>
tuple<Args...>
CSVParser<Args...>::
getRow()
{
  if (!stream_.eof())
    return parseLine(splitLine(readLine()));

  throw std::out_of_range("End of file is reached.");
}

template <class ...Args>
string
CSVParser<Args...>::
readLine()
{
  vector<string> lineVector;
  splitStream(stream_, rowDelim_, lineVector);

  if (lineVector.size() == 1) {
    return lineVector.front();
  }

  throw std::runtime_error("Couldn't read from input stream.");
}


template <class ...Args>
vector<string>
CSVParser<Args...>::
splitLine(const string& line)
{
  vector<string> rowItems;
  stringstream ss(line);
  splitStream(ss, colDelim_, rowItems, line.length());

  if (rowItems.size() != sizeof...(Args)) {
    string msg = "Invalid row length";
    throw std::invalid_argument(msg);
  }

  return rowItems;
}

template <class ...Args>
void
CSVParser<Args...>::
splitStream(std::istream& is, const char delimiter, vector<string>& items,
            size_t maxElements)
{
  bool escapeIsOn = false;
  string item;
  char c;

  while (!is.eof() && maxElements > 0) {
    is.get(c);

    if (!escapeIsOn && c == escapeChar_) {
      escapeIsOn = true;
      continue;
    }

    if ((!escapeIsOn && c == delimiter) || is.eof()) {
      if (item.length() > 0) {
        items.push_back(item);
        item = "";
        --maxElements;
      }
      continue;
    }

    item += c;
    escapeIsOn = false;
  }
}


template <class ...Args>
class CSVIterator {
  public:
    using iterator = CSVIterator<Args...>;
    using pointer_type = CSVParser<Args...>*;
    using return_type = tuple<Args...>;

    CSVIterator(pointer_type pointer, bool isEofReached);
    CSVIterator(const iterator&);
    iterator& operator=(const iterator&);

    bool operator==(const iterator&) const;
    bool operator!=(const iterator&) const;
    iterator operator++(int);
    iterator operator++();
//    return_type operator->() const;
    return_type operator*() const;

  private:
    pointer_type pointer_;
    return_type value_;
    bool isEofReached_;
    bool isInitialized_ = false;
};


template <class ...Args>
CSVIterator<Args...>::
CSVIterator(pointer_type pointer, bool isEofReached)
{
  pointer_ = pointer;
  isEofReached_ = isEofReached;

  try {
    if (!isEofReached)
      value_ = pointer_->getRow();
  } catch (std::out_of_range &e) {
    isEofReached_ = true;
  }
}


template <class ...Args>
CSVIterator<Args...>::
CSVIterator(const iterator& otherIt)
{
  pointer_ = otherIt.pointer_;
  isEofReached_ = otherIt.isEofReached_;
  isInitialized_ = otherIt.isInitialized_;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator&
CSVIterator<Args...>::
operator=(const iterator& otherIt)
{
  if (this != &otherIt) {
    pointer_ = otherIt.pointer_;
    isEofReached_ = otherIt.isEofReached_;
    isInitialized_ = otherIt.isInitialized_;
  }

  return *this;
}

template <class ...Args>
bool
CSVIterator<Args...>::
operator==(const iterator& otherIt) const
{
  return pointer_ == otherIt.pointer_ &&
         isEofReached_ == otherIt.isEofReached_;
}

template <class ...Args>
bool
CSVIterator<Args...>::
operator!=(const iterator& otherIt) const
{
  return !(*this == otherIt);
}

template <class ...Args>
typename CSVIterator<Args...>::iterator
CSVIterator<Args...>::
operator++()
{
  if (isEofReached_) throw std::out_of_range("");

  try {
    value_ = pointer_->getRow();
  } catch (std::out_of_range &e) {
    isEofReached_ = true;
  }

  return *this;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator
CSVIterator<Args...>::
operator++(int)
{
  if (isEofReached_) throw std::out_of_range("");
  iterator oldIt(*this);

  try {
    value_ = pointer_->getRow();
  } catch (std::out_of_range &e) {
    isEofReached_ = true;
  }

  return oldIt;
}

template <class ...Args>
typename CSVIterator<Args...>::return_type
CSVIterator<Args...>::
operator*() const
{
  if (isEofReached_) throw std::out_of_range("End of file is reached.");
  return value_;
}