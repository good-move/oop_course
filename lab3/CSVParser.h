#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

template <class ...Args>
class CSVParser;

template <class ...Args>
class CSVIterator;

enum IteratorState {
    Running,
    Finished
};

namespace {

  using std::stringstream;
  using std::string;
  using std::vector;
  using std::tuple;

//  utility functions that turn vector<string> into tuple with
//  corresponding types

  template <class T>
  tuple<T> wrap_in_tuple(const string& str) {
    T t;
    stringstream ss(str);
    ss >> t;
    if (ss.fail()) throw std::invalid_argument("Cannot parse string " + str);

    return tuple<T>{t};
  }

  template <>
  tuple<string> wrap_in_tuple(const string& str) {
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
    using string_vector = std::vector<std::string>;
    using csv_iterator = CSVIterator<Args...>;

    CSVParser(const std::string& fname,
              const char& rowDelim = '\n',
              const char& colDelim = ',',
              const char& escapeChar = '"');

    friend class CSVIterator<Args...>;
    csv_iterator begin() { return CSVIterator<Args...>{this, Running}; };
    csv_iterator end() { return CSVIterator<Args...>{this, Finished}; };

  private:
    std::tuple<Args...> getNextRow(std::fstream&, const size_t&) const;
    std::tuple<Args...> parseRow(const string_vector&) const;
    void split(std::fstream&, string_vector&) const;
    string_vector readRow(std::fstream&) const;

    std::string filename_;
    char escapeChar_;
    char rowDelim_;
    char colDelim_;
};

template <class ...Args>
CSVParser<Args...>::
CSVParser(const std::string& fname,
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
std::tuple<Args...>
CSVParser<Args...>::
getNextRow(std::fstream& stream, const size_t& rowNumber) const
{
  try {
    if (!stream.eof()) return parseRow(readRow(stream));
    throw std::out_of_range("End of file is reached.");
  } catch (std::invalid_argument& e) {
    stream.setstate(std::ios_base::iostate::_S_eofbit);

    throw std::invalid_argument(std::string("Error while parsing row ") +
                                std::to_string(rowNumber) + ", " + e.what());
  }
}

template <class ...Args>
typename CSVParser<Args...>::string_vector
CSVParser<Args...>::
readRow(std::fstream& stream) const
{
  string_vector row;
  split(stream, row);
  return row;
}

template <class ...Args>
std::tuple<Args...>
CSVParser<Args...>::
parseRow(const string_vector& items) const
{
  if (sizeof...(Args) != items.size())
    throw std::invalid_argument("invalid row length");

  return RowParser<Args...>::parse(0, items);
}

template <class ...Args>
void
CSVParser<Args...>::
split(std::fstream& stream, string_vector& items) const
{
  bool escapeIsOn = false;
  char c = escapeChar_;
  std::string item;

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
    using pointer_type = const CSVParser<Args...>*;
    using return_type = std::tuple<Args...>;
    using iterator = CSVIterator<Args...>;

    CSVIterator(pointer_type, IteratorState);
    CSVIterator(const iterator&);
    iterator& operator=(const iterator&);

    bool operator==(const iterator&) const;
    bool operator!=(const iterator&) const;
    return_type* operator->() const;
    return_type operator*() const;
    iterator operator++(int);
    iterator operator++();

  private:
    void copy(const iterator&);
    void iterate();

    // made mutable to allow gtell() calls in copy constructor/assignment
    mutable std::fstream stream_;
    pointer_type pointer_;
    return_type value_;
    IteratorState state_;
    size_t rowCount_;
};



template <class ...Args>
CSVIterator<Args...>::
CSVIterator(pointer_type pointer, IteratorState state)
{
  pointer_ = pointer;
  state_ = state;
  rowCount_ = 0;

  if (state == Running) {
    stream_.open(pointer->filename_);

    if (!stream_.is_open())
      throw std::runtime_error("Couldn't open file " + pointer->filename_);

    value_ = pointer_->getNextRow(stream_, ++rowCount_);
  }
}

template <class ...Args>
CSVIterator<Args...>::
CSVIterator(const iterator& otherIt)
{
  copy(otherIt);
}

template <class ...Args>
typename CSVIterator<Args...>::iterator&
CSVIterator<Args...>::
operator=(const iterator& otherIt)
{
  if (this != &otherIt) {
    stream_.close();
    copy(otherIt);
  }

  return *this;
}

template <class ...Args>
bool
CSVIterator<Args...>::
operator==(const iterator& otherIt) const
{
  return pointer_ == otherIt.pointer_ &&
         state_ == otherIt.state_ &&
         (state_ == Finished ? true : rowCount_ == otherIt.rowCount_);
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
  if (state_ == Finished) throw std::out_of_range("End of file is reached.");
  return value_;
}

template <class ...Args>
typename CSVIterator<Args...>::return_type*
CSVIterator<Args...>::
operator->() const
{
  if (state_ == Finished) throw std::out_of_range("End of file is reached.");
  return &value_;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator
CSVIterator<Args...>::
operator++()
{
  if (state_ == Finished) throw std::out_of_range("End of file is reached.");
  iterate();

  return *this;
}

template <class ...Args>
typename CSVIterator<Args...>::iterator
CSVIterator<Args...>::
operator++(int)
{
  if (state_ == Finished) throw std::out_of_range("End of file is reached.");
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
    value_ = pointer_->getNextRow(stream_, ++rowCount_);
  } catch (std::out_of_range& e) {
    state_ = Finished;
  }
}

template <class ...Args>
void
CSVIterator<Args...>::
copy(const iterator& otherIt)
{
  value_ = otherIt.value_;
  pointer_ = otherIt.pointer_;
  rowCount_ = otherIt.rowCount_;
  state_ = otherIt.state_;

  if (state_ == Running) {
    stream_.open(pointer_->filename_);

    if (!stream_.is_open())
      throw std::runtime_error("Copying error: couldn't open file " + pointer_->filename_);

    stream_.seekg(otherIt.stream_.tellg());
  }
}