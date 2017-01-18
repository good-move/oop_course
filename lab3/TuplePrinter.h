#pragma once

#include <tuple>
#include <iostream>

namespace {

  template<typename Type, unsigned I, unsigned Size>
  class TuplePrinter {
    public:
      static void print(std::ostream &os, const Type &value)
      {
        if (I > Size)
          throw std::invalid_argument(
                  "Error: tuple element index is greater than tuple size");

        os << std::get<I>(value) << ", ";
        TuplePrinter<Type, I + 1, Size>::print(os, value);
      }
  };

  template<typename Type, unsigned I>
  class TuplePrinter<Type, I, I> {
    public:
      static void print(std::ostream &os, const Type &value)
      {
        os << std::get<I>(value);
      }
  };

}

template <class Ch, class Tr, class ...Args>
std::basic_ostream<Ch, Tr>&
operator<< (std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& tuple) {
  TuplePrinter<std::tuple<Args...>, 0, sizeof...(Args) - 1>::print(os, tuple);
  return os;
}