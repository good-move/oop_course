#pragma once

#include <iostream>
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

    TritSetIterator& operator=(const TritSetIterator&);
    TritSetIterator& operator++();
    TritSetIterator operator++(int);
    bool operator==(const TritSetIterator&) const;
    bool operator!=(const TritSetIterator&) const;
    ValueType& operator*() const;
    ValueType* operator->() const;

  private:
    bool equals(const TritSetIterator&) const;

    PointerType iterablePtr_;
    ValueType* valuePtr_;
  };

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(PointerClass pointer, size_t startIndex)
  {
    if (pointer == nullptr) {
      throw std::invalid_argument("Cannot initialize iterator from nullptr");
    }
    iterablePtr_ = pointer;
    valuePtr_ = new ValueType(*iterablePtr_, startIndex);
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(const TritSetIterator& otherIt)
  {
    iterablePtr_ = otherIt.iterablePtr_;
    valuePtr_ = new ValueType(*otherIt.valuePtr_);
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  TritSetIterator(TritSetIterator&& otherIt)
  {
    iterablePtr_ = otherIt.iterablePtr_;
    valuePtr_ = otherIt.valuePtr_;
    otherIt.iterablePtr_ = nullptr;
    otherIt.valuePtr_ = nullptr;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>::
  ~TritSetIterator()
  {
    delete valuePtr_;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
  operator=(const TritSetIterator<PointerClass, ValueType>& otherIt)
  {
    if (this != &otherIt) {
      iterablePtr_ = otherIt.iterablePtr_;
      ValueType* tmpPtr = new ValueType(*otherIt.valuePtr_);

      delete valuePtr_;
      valuePtr_ = tmpPtr;
    }

    return *this;
  }

  template <class PointerClass, class ValueType>
  ValueType& TritSetIterator<PointerClass, ValueType>::
  operator*() const
  {
    if (valuePtr_->getIndex() >= iterablePtr_->length()) {
      throw std::out_of_range("Cannot access element beyond iterable length");
    }

    return *valuePtr_;
  }

  template <class PointerClass, class ValueType>
  ValueType* TritSetIterator<PointerClass, ValueType>::
  operator->() const
  {
    if (valuePtr_->getIndex() >= iterablePtr_->length()) {
      throw std::out_of_range("Cannot access element beyond iterable length");
    }

    return valuePtr_;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
  operator++()
  {
    valuePtr_->setIndex(valuePtr_->getIndex() + 1);

    return *this;
  }

  template <class PointerClass, class ValueType>
  TritSetIterator<PointerClass, ValueType> TritSetIterator<PointerClass, ValueType>::
  operator++(int)
  {
    TritSetIterator oldIt(*this);
    valuePtr_->setIndex(valuePtr_->getIndex() + 1);

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
    return iterablePtr_ == otherIt.iterablePtr_ &&
           valuePtr_->getIndex() == otherIt.valuePtr_->getIndex() &&
           iterablePtr_->length() == otherIt.iterablePtr_->length();
  }

} // namespace alexgm
