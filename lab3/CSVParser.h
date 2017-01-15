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

//  utility functions that turn vector<string> into tuple with
//  corresponding types

  template <class T>
  tuple<T> wrap_in_tuple(const string &str) {
    T t;
    stringstream ss(str);
    ss >> t;
    if (ss.fail()) throw std::invalid_argument("Cannot parse string " + str);

    return tuple<T>{t};
  }

  template <>
  tuple<string> wrap_in_tuple(const string &str) {
    return tuple<string>{str};
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
          return wrap_in_tuple<T>(items.at(index));
        } catch (std::invalid_argument&){
          throw std::invalid_argument("column " + std::to_string(index+1));
        }
      };
  };

  template <class T, class ...Types>
  struct RowParser<T, Types...> {
      static tuple<T, Types...> parse(size_t index, const vector<string>& items) {
        tuple<T> tuple;
        try {
          tuple = wrap_in_tuple<T>(items.at(index));
        } catch (std::invalid_argument&){
          throw std::invalid_argument("column " + std::to_string(index+1));
        }
        return std::tuple_cat(tuple, RowParser<Types...>::parse(index + 1, items));
      }
  };
}


template <class ...Args>
class CSVParser {
  public:
    using csv_iterator = CSVIterator<Args...>;

    CSVParser(const string& fname,
              const char& rowDelim = '\n',
              const char& colDelim = ',',
              const char& escapeChar = '"');

    friend class CSVIterator<Args...>;
    csv_iterator begin() { return CSVIterator<Args...>{this, false, filename_}; };
    csv_iterator end() { return CSVIterator<Args...>{this, true, filename_}; };

  private:
    tuple<Args...> parseRow(const vector<string>&) const;
    tuple<Args...> getNextRow(ifstream&, size_t) const;
    void split(ifstream&, vector<string>&) const;
    vector<string> readRow(ifstream&) const;

    string filename_;
    char escapeChar_;
    char rowDelim_;
    char colDelim_;
};

template <class ...Args>
CSVParser<Args...>::
CSVParser(const string& fname,
          const char& rowDelim,
          const char& colDelim,
          const char& escapeChar)
{
  filename_ = fname;
  colDelim_ = colDelim;
  rowDelim_ = rowDelim;
  escapeChar_ = escapeChar;
}

template <class ...Args>
tuple<Args...>
CSVParser<Args...>::
getNextRow(ifstream& stream, size_t rowNumber) const
{
  try {
    if (!stream.eof()) {
      return parseRow(readRow(stream));
    }
  } catch (std::invalid_argument& e) {
    stream.setstate(std::ios_base::iostate::_S_eofbit);

    throw std::invalid_argument(string("Error while parsing row ") +
                                std::to_string(rowNumber) + ", " + e.what());
  }
  throw std::out_of_range("End of file is reached.");
}

template <class ...Args>
vector<string>
CSVParser<Args...>::
readRow(ifstream& stream) const
{
  vector<string> row;
  split(stream, row);
  return row;
}

template <class ...Args>
tuple<Args...>
CSVParser<Args...>::
parseRow(const vector<string> &items) const
{
  if (sizeof...(Args) != items.size())
    throw std::invalid_argument("invalid row length");

  return RowParser<Args...>::parse(0, items);
}

template <class ...Args>
void
CSVParser<Args...>::
split(ifstream& stream, vector<string>& items) const
{
  bool escapeIsOn = false;
  char c = escapeChar_;
  string item;

  while (!stream.eof()) {
    stream.get(c);

    if (c == escapeChar_) {
      if (escapeIsOn) {
        stream.get(c);
        escapeIsOn = (c == escapeChar_ && !stream.eof());
      } else {
        escapeIsOn = true;
        continue;
      }
    }

    if ((!escapeIsOn && (c == colDelim_ || c == rowDelim_)) ||
        stream.eof()
        ) {
      items.push_back(item);
      item = "";
      if (c == rowDelim_) break;
      else continue;
    }

    item += c;
  }

  if (escapeIsOn) throw std::logic_error("Escape sequence is not closed");
}


template <class ...Args>
class CSVIterator {
  public:
    using iterator = CSVIterator<Args...>;
    using pointer_type = CSVParser<Args...>*;
    using return_type = tuple<Args...>;

    CSVIterator(pointer_type, bool, const string&);
    CSVIterator(const iterator&);
    iterator& operator=(const iterator&);

    bool operator==(const iterator&) const;
    bool operator!=(const iterator&) const;
    return_type* operator->() const;
    return_type operator*() const;
    iterator operator++(int);
    iterator operator++();

  private:
    void iterate();

    pointer_type pointer_;
    return_type value_;
    bool isEofReached_;
    ifstream stream_;
    size_t rowCount_;
};



template <class ...Args>
CSVIterator<Args...>::
CSVIterator(pointer_type pointer, bool isEofReached, const string& filename)
{
  pointer_ = pointer;
  isEofReached_ = isEofReached;
  stream_.open(filename);
  rowCount_ = 1;

  if (!stream_.is_open())
    throw std::runtime_error("Couldn't open file " + filename);

  if (!isEofReached) {
    value_ = pointer_->getNextRow(stream_, rowCount_++);
  }
}

template <class ...Args>
CSVIterator<Args...>::
CSVIterator(const iterator& otherIt)
{
  value_ = otherIt.value_;
  pointer_ = otherIt.pointer_;
  isEofReached_ = otherIt.isEofReached_;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator&
CSVIterator<Args...>::
operator=(const iterator& otherIt)
{
  if (this != &otherIt) {
    value_ = otherIt.value_;
    pointer_ = otherIt.pointer_;
    isEofReached_ = otherIt.isEofReached_;
  }

  return *this;
}

template <class ...Args>
bool
CSVIterator<Args...>::
operator==(const iterator& otherIt) const
{
  return pointer_ == otherIt.pointer_ &&
          (isEofReached_ == otherIt.isEofReached_?
           true : rowCount_ == otherIt.rowCount_);
}

template <class ...Args>
bool
CSVIterator<Args...>::
operator!=(const iterator& otherIt) const
{
  return !(*this == otherIt);
}

template <class ...Args>
typename CSVIterator<Args...>::return_type
CSVIterator<Args...>::
operator*() const
{
  if (isEofReached_) throw std::out_of_range("End of file is reached.");
  return value_;
}

template <class ...Args>
typename CSVIterator<Args...>::return_type*
CSVIterator<Args...>::
operator->() const
{
  if (isEofReached_) throw std::out_of_range("End of file is reached.");
  return &value_;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator
CSVIterator<Args...>::
operator++()
{
  if (isEofReached_) throw std::out_of_range("End of file is reached.");
  iterate();

  return *this;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator
CSVIterator<Args...>::
operator++(int)
{
  if (isEofReached_) throw std::out_of_range("End of file is reached.");
  iterator oldIt(*this);

  iterate();

  return oldIt;
}

template <class ...Args>
void
CSVIterator<Args...>::
iterate()
{
  try {
    value_ = pointer_->getNextRow(stream_, rowCount_++);
  } catch (std::out_of_range& e) {
    isEofReached_ = true;
  }
}