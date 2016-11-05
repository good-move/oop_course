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
    size_t length_;

  };

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(PointerClass pointer, size_t startIndex) : pointer_(pointer), index_(startIndex), length_(pointer->length())
  {}

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(const TritSetIterator& otherIt) : pointer_(otherIt.pointer_), index_(otherIt.index_), length_(otherIt.length_)
  {}

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(TritSetIterator&& otherIt)
  {
    pointer_ = otherIt.pointer_;
    otherIt.pointer_ = nullptr;

    index_ = otherIt.index_;
    length_ = otherIt.length_;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  ~TritSetIterator()
  {}

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
  operator=(const TritSetIterator<PointerClass, ValueType> otherIt)
  {
    this->pointer_ = otherIt.pointer_;
    this->index_ = otherIt.index_;
    this->length_ = otherIt.length_;

    return *this;
  }

  template <class PointerClass, class ValueType>
  ValueType& TritSetIterator<PointerClass, ValueType>::
  operator*() const
  {
    if (index_ >= length_) {
      throw std::out_of_range("Cannot access element beyond iterable length");
    }
    return (*pointer_)[index_];
  }

  template <class PointerClass, class ValueType>
  ValueType& TritSetIterator<PointerClass, ValueType>::
  operator->() const
  {
    if (index_ >= length_) {
      throw std::out_of_range("Cannot access element beyond iterable length");
    }
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
    return pointer_ == otherIt.pointer_ &&
           index_ == otherIt.index_ &&
           length_ == otherIt.length_;
  }

} // namespace alexgm
