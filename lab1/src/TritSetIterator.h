#pragma once

#include <stdexcept>
#include <cstdlib>

namespace alexgm {

  using namespace std;

  template <class PointerType, class ValueType> class TritSetIterator {

  public:
    TritSetIterator(PointerType, size_t);
    TritSetIterator(const TritSetIterator&);
    TritSetIterator(TritSetIterator&&);
    ~TritSetIterator();

    TritSetIterator& operator=(const TritSetIterator);
    TritSetIterator& operator++();
    TritSetIterator operator++(int);
    bool operator==(const TritSetIterator&) const;
    bool operator!=(const TritSetIterator&) const;
    ValueType& operator*() const;
    ValueType& operator->() const;

  private:
    bool equals(const TritSetIterator&) const;

    PointerType pointer_;
    size_t index_;

  };

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(PointerClass pointer, size_t startIndex) : pointer_(pointer), index_(startIndex)
  {}

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(const TritSetIterator& otherIt) : pointer_(otherIt.pointer_), index_(otherIt.index_)
  {}

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(TritSetIterator&& otherIt)
  {
    pointer_ = otherIt.pointer_;
    otherIt.pointer_ = nullptr;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  ~TritSetIterator()
  {}

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
  operator=(const TritSetIterator<PointerClass, ValueType> otherIt)
  {
    TritSetIterator<PointerClass, ValueType> tmp(otherIt);
    *this = std::move(tmp);
    return *this;
  }

  template <class PointerClass, class ValueType>
  ValueType& TritSetIterator<PointerClass, ValueType>::
  operator*() const
  {
//    cout << "next value: " << (*pointer_)[index_] << endl;
    return (*pointer_)[index_];
  }

  template <class PointerClass, class ValueType>
  ValueType& TritSetIterator<PointerClass, ValueType>::
  operator->() const
  {
    return (*pointer_)[index_];

  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
  operator++()
  {
    index_++;
    return *this;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType> TritSetIterator<PointerClass, ValueType>::
  operator++(int)
  {
    TritSetIterator oldIt(*this);
    index_++;
    return oldIt;
  }

  template <class PointerClass, class ValueType>
  bool TritSetIterator<PointerClass, ValueType>::
  operator==(const TritSetIterator<PointerClass, ValueType>& otherIt) const
  {
    return this->equals(otherIt);
  }

  template <class PointerClass, class ValueType>
  bool TritSetIterator<PointerClass, ValueType>::
  operator!=(const TritSetIterator<PointerClass, ValueType>& otherIt) const
  {
    return !this->equals(otherIt);
  }

  template <class PointerClass, class ValueType>
  bool TritSetIterator<PointerClass, ValueType>::
  equals(const TritSetIterator& otherIt) const
  {
    return pointer_ == otherIt.pointer_ && index_ == otherIt.index_;
  }

} // namespace alexgm
