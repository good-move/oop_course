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

  enum Trit { False=-1, Unknown=0, True=1 }; // 00, 01, 10

  class TritSet {

    class TritHolder {
      public:
        TritHolder(const size_t);
        ~TritHolder();
        operator Trit();
        Trit& operator= (Trit);
        bool operator== (Trit);
        void resize(const size_t);
      private:
        friend class TritSet;
        uint* tritSet_;
        size_t index_;
        size_t allocTritSetLength_;
        size_t tritSetLength_; // num of trits
        size_t arrayLength_; // uint's for tritSetLength_ trits
        size_t lastSet_;

        size_t computeArrayLength(const size_t);
        void initArray(uint[], const size_t);
        void findLastSet();
        void cleanAfter(const size_t);
    };

    public:
      TritSet(const TritSet&);
      explicit TritSet(const size_t);
      ~TritSet();
      TritHolder& operator[] (const size_t);
      TritSet& operator&= (TritSet const&);
      TritSet operator& (TritSet const&);
      TritSet& operator= (TritSet const&);
      // unordered_map< Trit, int, hash<int> > cardinality();
      size_t cardinality(Trit) const;
      size_t capacity() const;
      size_t length() const;
      void shrink();
      void trim(size_t);
    private:
      TritHolder* tritHolder_;
  };

}

#endif // TritSet_H
