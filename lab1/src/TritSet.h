#pragma once

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <new>

using namespace std;

namespace alexgm {

  enum Trit : uint { False=0, Unknown=2, True=3 }; // 00, 10, 11

  Trit& operator!(Trit);

  class TritSet;

  const uint BITS_PER_UINT = sizeof(uint) * 8u;
  const uint BITS_PER_TRIT = 2u;
  const uint TRIT_STAMP = 3u;

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
          size_t computeArrayLength(const size_t);
          void initArray(uint[], const size_t);
          void cleanAfter(const size_t);
          uint getPlaceholder();
          void findLastSet();
          bool equals(Trit);
          Trit getTrit(size_t, size_t);
          Trit getTrit();
          void setTrit(uint);

          friend class TritSet;

          uint* tritSet_;
          size_t lastAccessedInd_;
          size_t allocTritSetLength_;
          size_t tritSetLength_; // num of trits
          size_t arrayLength_; // uint's for tritSetLength_ trits
          size_t maxSetInd_;
    };

    public:
      TritSet(const TritSet&);
      TritSet(const size_t);
      TritSet(TritSet&&);
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
//      unordered_map< Trit, int, hash<int> > cardinality();
      size_t cardinality(Trit) const;
      size_t capacity() const;
      void trim(const size_t);
      size_t length() const;
      void shrink();

    private:
      bool equals(const TritSet&);

      TritHolder* tritHolder_;
  };

}
