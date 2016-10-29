#pragma once

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <new>
#include <unordered_map>

#include "TritSetIterator.h"

using namespace std;

namespace alexgm {

  enum Trit : uint { False=0, Unknown=2, True=3 }; // 00, 10, 11

  Trit& operator!(Trit);

  class TritSet;
  class TritHash;

  const uint BITS_PER_UINT = sizeof(uint) * 8u;
  const uint BITS_PER_TRIT = 2u;
  const uint TRIT_STAMP = 3u;



  class TritSet {

    class TritHolder;

    typedef TritSetIterator<const TritSet*, const TritHolder> const_iterator;
    typedef TritSetIterator<TritSet*, TritHolder> iterator;

    public:

      iterator begin();
      iterator end();

      const_iterator begin() const;
      const_iterator end() const;

      TritSet(const size_t);
      TritSet(const TritSet&);
      TritSet(TritSet&&);
      ~TritSet();

      TritHolder& operator[] (const size_t) const;
      TritSet& operator&= (const TritSet&);
      TritSet& operator|= (const TritSet&);
      TritSet& operator= (TritSet);
      TritSet operator& (const TritSet&);
      TritSet operator| (const TritSet&);
      bool operator== (const TritSet&);
      bool operator!= (const TritSet&);
      TritSet& operator~ ();
      unordered_map< Trit, size_t, TritHash > cardinality();
      size_t cardinality(Trit) const;
      size_t capacity() const;
      void trim(const size_t);
      size_t length() const;
      void shrink();

    private:
      bool equals(const TritSet&);

      TritHolder* tritHolder_;

      class TritHolder {
      public:
          TritHolder(const size_t);
          TritHolder(const TritHolder&);
          TritHolder(TritHolder&&);
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
          void findMaxSetInd();
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

  };

  class TritHash {
    public: inline uint operator() (const Trit& trit) const { return trit == False? 0u : trit-1u; };
  };

}
