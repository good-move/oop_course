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

  enum Trit : uint { False=0, Unknown=2, True=3 }; // 00, 10, 11

  Trit& operator!(Trit);

  class TritSet {

    class TritHolder {
      public:
        TritHolder(const size_t);
        ~TritHolder();

        void resize(const size_t);
        Trit& operator= (uint);
        bool operator== (Trit);
        bool operator!= (Trit);
        Trit operator! ();
        operator Trit();

      private:
        friend class TritSet;
        uint* tritSet_;
        size_t allocTritSetLength_;
        size_t tritSetLength_; // num of trits
        size_t arrayLength_; // uint's for tritSetLength_ trits
        size_t lastSet_;
        size_t index_;

        size_t computeArrayLength(const size_t);
        void initArray(uint[], const size_t);
        void cleanAfter(const size_t);
        uint getPlaceholder();
        void findLastSet();
        bool equals(Trit);
    };

    public:
      TritSet(TritSet&&);
      TritSet(const TritSet&);
      TritSet(const size_t);
      ~TritSet();

      TritHolder& operator[] (const size_t);
      TritSet& operator&= (const TritSet&);
      TritSet& operator|= (const TritSet&);
      TritSet& operator= (TritSet const&);
      TritSet operator& (const TritSet&);
      TritSet operator| (const TritSet&);
      bool operator== (const TritSet&);
      bool operator!= (const TritSet&);
      TritSet& operator~ ();
      // unordered_map< Trit, int, hash<int> > cardinality();
      size_t cardinality(Trit) const;
      size_t capacity() const;
      void trim(const size_t);
      size_t length() const;
      void shrink();

    private:
      TritHolder* tritHolder_;

      bool equals(const TritSet&);
  };

}

#endif // TritSet_H
