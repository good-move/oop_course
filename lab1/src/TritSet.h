#pragma once

#include "TritSetIterator.h"

#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <new>

using namespace std;

namespace alexgm {

  enum Trit : uint { False=0, Unknown=2, True=3 }; // 00, 10, 11

  Trit operator!(Trit);

  class TritSet;
  class TritHash;

  const uint BITS_PER_UINT = sizeof(uint) * 8u;
  const uint BITS_PER_TRIT = 2u;
  const uint TRIT_STAMP = 3u;

  class TritSet {

    class TritHolder;

    public:

      typedef TritSetIterator<const TritSet*, const TritHolder> const_iterator;
      typedef TritSetIterator<TritSet*, TritHolder> iterator;

      iterator begin();
      iterator end();

      const_iterator begin() const;
      const_iterator end() const;

      TritSet();
      TritSet(const size_t);
      TritSet(const TritSet&);
      TritSet(TritSet&&);
      ~TritSet();

      TritHolder& operator[] (const size_t) const;
      TritSet& operator&= (const TritSet&);
      TritSet& operator|= (const TritSet&);
      TritSet& operator= (const TritSet&);
      TritSet operator& (const TritSet&) const;
      TritSet operator| (const TritSet&) const;
      bool operator== (const TritSet&) const;
      bool operator!= (const TritSet&) const;
      TritSet operator~ ();
      unordered_map< Trit, size_t, TritHash > cardinality() const;
      size_t cardinality(Trit) const;
      size_t capacity() const;
      void trim(const size_t);
      size_t length() const;
      void shrink();

    private:
      bool equals(const TritSet&) const;

      TritHolder* tritHolder_;

        class TritHolder {
        public:
          TritHolder(const size_t);
          TritHolder(const TritHolder&);
          TritHolder(TritHolder&&);
          ~TritHolder();

          TritHolder& operator= (const Trit);
          TritHolder& operator= (const TritHolder);
          bool operator== (Trit) const;
          bool operator!= (Trit) const;
          Trit operator! ();
          operator Trit() const;

        private:
          size_t computeArrayLength(const size_t);
          void initArray(uint[], const size_t);
          void cleanAfter(const size_t);
          void resize(const size_t);
          uint getPlaceholder();
          void findMaxSetInd();
          void copyFrom(TritHolder);
          bool equals(Trit) const;
          Trit getTrit(size_t, size_t) const;
          Trit getTrit() const;
          void setTrit(uint);
          TritHolder& copyAssignment(const Trit);

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
