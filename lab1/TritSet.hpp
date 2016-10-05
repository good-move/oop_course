#ifndef TritSet_H
#define TritSet_H

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cmath>
#include <list>
#include <new>

using namespace std;

namespace alexgm {

  enum Trit { False, Unknown, True }; // 00, 01, 10

  class TritSet {

    class TritHolder {
      public:
        TritHolder(const size_t);
        ~TritHolder();
        operator Trit();
        Trit& operator= (Trit);
        Trit& operator== (Trit);
        void resize(const size_t);
        void setCurrentIndex(const size_t);
        size_t getTritSetLength();
      private:
        uint* tritSet_;
        size_t index_;
        size_t allocTritSetLength_;
        size_t tritSetLength_; // num of trits
        size_t arrayLength_; // uint's for length_ trits
        size_t lastSet_;

        size_t computeArrayLength(const size_t);
    };

    public:
      explicit TritSet(const size_t);
      ~TritSet();
      TritHolder& operator[] (const size_t);
      Trit operator& (Trit&);
      // unordered_map< Trit, int, hash<int> > cardinality();
      size_t cardinality(Trit);
      size_t capacity();
      size_t length();
      void shrink();
      void trim(size_t);
    private:
      TritHolder* tritHolder_;
  };

}

#endif // TritSet_H
