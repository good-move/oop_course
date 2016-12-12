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
    class ConstTritHolder;

    public:
      using const_iterator = TritSetIterator<const TritSet*, ConstTritHolder>;
      using iterator = TritSetIterator<TritSet*, TritHolder>;

      iterator begin();
      iterator end();

      const_iterator begin() const;
      const_iterator end() const;

      TritSet();
      TritSet(const size_t);
      TritSet(const TritSet&);
      TritSet(TritSet&&);
      ~TritSet();

      ConstTritHolder operator[] (const size_t) const;
      TritHolder operator[] (const size_t);
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

      friend ostream& operator<<(ostream& os, const TritSet& tritSet);

    private:
      static uint getPlaceholder();
      static inline uint conjunction(uint a, uint b) { return a & b; };
      static inline uint disjunction(uint a, uint b) { return a | b; };

      void copyStateFrom(const TritSet&);
      size_t computeArrayLength(const size_t) const;
      void initArray(uint[], const size_t);
      void cleanAfter(const size_t);
      void resize(const size_t);
      void findMaxSetInd();
      bool equals(const TritSet&) const;
      Trit getTrit(size_t, size_t) const;
      Trit getTrit(size_t) const;
      void setTrit(size_t, Trit);
      size_t countUintIndex(size_t) const;
      size_t countOffset(size_t) const;
      size_t capacityToLength(size_t) const;
      void logicOperation(const TritSet&, uint(*)(uint, uint));

      uint* tritSet_;
      size_t allocLength_;
      size_t currentLength_; // num of trits
      size_t arrayLength_; // uint's for currentLength_ trits
      size_t maxSetInd_;

      class TritHolder {
      public:
        TritHolder(TritSet&, const size_t);
        TritHolder(const TritHolder&);
        TritHolder(TritHolder&&);

        TritHolder& operator= (const TritHolder);
        TritHolder& operator= (const Trit);
        bool operator== (Trit) const;
        bool operator!= (Trit) const;
        Trit operator!() const;
        operator Trit() const;

        size_t getIndex() const;
        void setIndex(size_t);

      private:
        bool equals(Trit) const;

        TritSet& parent_;
        size_t tritIndex_;
      };

      class ConstTritHolder {
      public:
        ConstTritHolder(const TritSet&, const size_t);
        ConstTritHolder(const ConstTritHolder&);
        ConstTritHolder(ConstTritHolder&&);

        bool operator== (Trit) const;
        bool operator!= (Trit) const;
        Trit operator!() const;
        operator Trit() const;

        size_t getIndex() const;
        void setIndex(size_t);

      private:
        bool equals(Trit) const;

        const TritSet& parent_;
        size_t tritIndex_;
      };

  };

  class TritHash {
    public: inline uint operator() (const Trit& trit) const { return trit == False? 0u : trit-1u; };
  };

}
