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
class CSVParser{
  public:
    using csv_iterator = CSVIterator<Args...>;

    CSVParser(const string& fname,
              const char& rowDelim = '\n',
              const char& colDelim = ',',
              const char& escapeChar = '"');

    tuple<Args...> getNextRow();
    tuple<Args...> getCachedRow();

    csv_iterator begin() { return csv_iterator(this, false); };
    csv_iterator end() { return csv_iterator(this, true); };

  private:
    void split(vector<string>&);
    vector<string> readRow();

    tuple<Args...> parseRow(const vector<string>& items) {
      if (sizeof...(Args) != items.size())
        throw std::invalid_argument("invalid row length");
      return RowParser<Args...>::parse(0, items);
    };

    tuple<Args...> cached_;
    std::ifstream stream_;
    size_t rowsRead_ = 0;
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
  colDelim_ = colDelim;
  rowDelim_ = rowDelim;
  escapeChar_ = escapeChar;
  stream_.open(fname);

  if (stream_.fail()) throw std::runtime_error("Couldn't open file " + fname);
}

template <class ...Args>
tuple<Args...>
CSVParser<Args...>::
getNextRow()
{
  try {
    if (!stream_.eof()) {
      cached_ = parseRow(readRow());
      return cached_;
    }
  } catch (std::invalid_argument& e) {
    stream_.setstate(std::ios_base::iostate::_S_eofbit);
    throw std::invalid_argument(string("Error while parsing row ") +
                                std::to_string(rowsRead_) + ", " + e.what());
  }
  throw std::out_of_range("End of file is reached.");
}

template <class ...Args>
tuple<Args...>
CSVParser<Args...>::
getCachedRow()
{
  return (rowsRead_ == 0) ? getNextRow() : cached_;
}

template <class ...Args>
vector<string>
CSVParser<Args...>::
readRow()
{
  vector<string> row;
  split(row);
  rowsRead_++;
  return row;
}

template <class ...Args>
void
CSVParser<Args...>::
split(vector<string>& items)
{
  bool escapeIsOn = false;
  char c = escapeChar_;
  string item;

  while (!stream_.eof()) {
    stream_.get(c);

    if (c == escapeChar_) {
      if (escapeIsOn) {
        stream_.get(c);
        escapeIsOn = (c == escapeChar_ && !stream_.eof());
      } else {
        escapeIsOn = true;
        continue;
      }
    }

    if ((!escapeIsOn && (c == colDelim_ || c == rowDelim_)) ||
        stream_.eof()
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

    CSVIterator(pointer_type pointer, bool isEofReached);
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
};



template <class ...Args>
CSVIterator<Args...>::
CSVIterator(pointer_type pointer, bool isEofReached)
{
  pointer_ = pointer;
  isEofReached_ = isEofReached;

  try {
    value_ = pointer_->getCachedRow();
  } catch (std::out_of_range &e) {
    std::cout << e.what() << std::endl;
    isEofReached_ = true;
  } catch (std::invalid_argument& e) {
    isEofReached_ = true;
    std::cout << e.what() << std::endl;
  }
}

template <class ...Args>
CSVIterator<Args...>::
CSVIterator(const iterator& otherIt)
{
  pointer_ = otherIt.pointer_;
  isEofReached_ = otherIt.isEofReached_;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator&
CSVIterator<Args...>::
operator=(const iterator& otherIt)
{
  if (this != &otherIt) {
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
    value_ = pointer_->getNextRow();
  } catch (std::out_of_range& e) {
    isEofReached_ = true;
  } catch (std::invalid_argument& e) {
    std::cout << e.what();
    isEofReached_ = true;
  }
}