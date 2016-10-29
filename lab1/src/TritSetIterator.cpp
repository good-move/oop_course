#include "TritSetIterator.h"

using namespace alexgm;

template <class PointerClass, typename ValueType>
TritSetIterator<PointerClass, ValueType>::
TritSetIterator(PointerClass pointer, size_t startIndex) : pointer_(pointer), index_(startIndex)
{}

template <class PointerClass, typename ValueType>
TritSetIterator<PointerClass, ValueType>::
TritSetIterator(const TritSetIterator& otherIt) : pointer_(otherIt.pointer_), index_(otherIt.index_)
{}

template <class PointerClass, typename ValueType>
TritSetIterator<PointerClass, ValueType>::
TritSetIterator(TritSetIterator&& otherIt)
{
  pointer_ = otherIt.pointer_;
  otherIt.pointer_ = nullptr;
}

template <class PointerClass, typename ValueType>
ValueType& TritSetIterator<PointerClass, ValueType>::
operator*() const
{
  return pointer_[index_];
}

template <class PointerClass, typename ValueType>
ValueType& TritSetIterator<PointerClass, ValueType>::
operator->() const
{
  return pointer_[index_];

}

template <class PointerClass, typename ValueType>
TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
operator++()
{
  index_++;
  return *this;
}

template <class PointerClass, typename ValueType>
TritSetIterator<PointerClass, ValueType>& TritSetIterator<PointerClass, ValueType>::
operator++(int)
{
  TritSetIterator oldIt(*this);
  index_++;
  return oldIt;
}

template <class PointerClass, typename ValueType>
bool TritSetIterator<PointerClass, ValueType>::
operator==(const TritSetIterator<PointerClass, ValueType>& otherIt) const
{
  return this->equals(otherIt);
}

template <class PointerClass, typename ValueType>
bool TritSetIterator<PointerClass, ValueType>::
operator!=(const TritSetIterator<PointerClass, ValueType>& otherIt) const
{
  return !this->equals(otherIt);
}

template <class PointerClass, typename ValueType>
bool TritSetIterator<PointerClass, ValueType>::
equals(const TritSetIterator& otherIt) const
{
  return pointer_ == otherIt.pointer_ && index_ == otherIt.index_;
}
