#include "TritSet.hpp"

namespace alexgm {

  /* **************************  TritSet functions ************************** */

  TritSet::
  TritSet(const size_t length)
  {
    tritHolder_ = new TritHolder(length);
  }

  TritSet:: // copy constructor
  TritSet(const TritSet& tritSet)
  {
    tritHolder_ = new TritHolder(tritSet.capacity() * sizeof(uint) * 4);

    TritHolder* outerTh = tritSet.tritHolder_;
    tritHolder_->index_               = outerTh->index_;
    tritHolder_->allocTritSetLength_  = outerTh->allocTritSetLength_;
    tritHolder_->tritSetLength_       = outerTh->tritSetLength_;
    tritHolder_->arrayLength_         = outerTh->arrayLength_;
    tritHolder_->lastSet_             = outerTh->lastSet_;
    memcpy(tritHolder_->tritSet_, outerTh->tritSet_, sizeof(uint) * tritSet.capacity());
  }

  TritSet:: // move contructor
  TritSet(TritSet&& outerSet)
  {
    this->tritHolder_ = outerSet.tritHolder_;
    outerSet.tritHolder_ = nullptr;
  }

  TritSet::
  ~TritSet()
  {
    if (tritHolder_ != nullptr)
      delete tritHolder_;
    tritHolder_ = nullptr;
  }

  TritSet::TritHolder& TritSet::
  operator[](const size_t index)
  {
    tritHolder_->index_ = index;

    return *tritHolder_;
  }

  size_t TritSet::
  capacity() const
  {
    return tritHolder_->arrayLength_;
  }

  size_t TritSet::
  length() const
  {
    return tritHolder_->lastSet_ + 1;
  }

  void TritSet::
  shrink()
  {
    size_t minSize = 0;
    if (tritHolder_->lastSet_+1 < tritHolder_->allocTritSetLength_) {
      minSize = tritHolder_->lastSet_+1;
    } else {
      minSize = tritHolder_->allocTritSetLength_;
    }

    this->trim(minSize);
  }

  void TritSet::
  trim(const size_t lastIndex)
  {
    if (lastIndex >= 0 && lastIndex < tritHolder_->tritSetLength_) {
      tritHolder_->resize(lastIndex);
      tritHolder_->cleanAfter(lastIndex);
      tritHolder_->findLastSet();
    }
  }

  TritSet& TritSet::
  operator= (const TritSet& outerSet)
  {
    if (this != &outerSet) {
      TritSet tmp(outerSet);
      std::swap(*this, tmp);
    }
    return *this;
  }

  TritSet& TritSet::
  operator&= (const TritSet& outerSet)
  {
    uint placeholder = this->tritHolder_->getPlaceholder();
    size_t maxCapacity = (this->capacity() > outerSet.capacity()? this->capacity() : outerSet.capacity());
    this->tritHolder_->resize(maxCapacity * sizeof(uint) * 4);

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        this->tritHolder_->tritSet_[i] &= outerSet.tritHolder_->tritSet_[i];
      } else {
        this->tritHolder_->tritSet_[i] &= placeholder; // implies ...[i] &= Unknown
      }
    }

    return *this;
  }

  TritSet TritSet::
  operator& (const TritSet& outerSet)
  {
    TritSet tmp(*this);
    tmp &= outerSet;
    return tmp;
  }

  TritSet& TritSet::
  operator|= (const TritSet& outerSet)
  {

    uint placeholder = this->tritHolder_->getPlaceholder();
    size_t maxCapacity = (this->capacity() > outerSet.capacity()? this->capacity() : outerSet.capacity());
    this->tritHolder_->resize(maxCapacity * sizeof(uint) * 4);

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        this->tritHolder_->tritSet_[i] |= outerSet.tritHolder_->tritSet_[i];
      } else {
        this->tritHolder_->tritSet_[i] |= placeholder; // implies ...[i] |= Unknown
      }
    }

    return *this;
  }

  TritSet TritSet::
  operator| (const TritSet& outerSet)
  {
    TritSet tmp(*this);
    tmp |= outerSet;
    return tmp;
  }

  bool TritSet::
  operator== (const TritSet& outerSet)
  {
    return this->equals(outerSet);
  }

  bool TritSet::
  operator!= (const TritSet& outerSet)
  {
    return !this->equals(outerSet);
  }

  TritSet& TritSet::
  operator~()
  {
    for (size_t i = 0; i < this->length(); ++i)
      (*this)[i] = !((*this)[i]);

    return *this;
  }

  bool TritSet::
  equals(const TritSet& outerSet)
  {
    if (this->capacity() != outerSet.capacity())
      return false;

    uint* innerTritSet = this->tritHolder_->tritSet_;
    uint* outerTritSet = outerSet.tritHolder_->tritSet_;

    for (size_t i = 0; i < this->capacity(); ++i) {
      if (innerTritSet[i] != outerTritSet[i]) {
        return false;
      }
    }
    return true;
  }

/* ************************* TritHolder functions ************************* */

  TritSet::TritHolder::
  TritHolder(const size_t length)
  {
    allocTritSetLength_ = length;
    tritSetLength_ = length;
    lastSet_ = 0;

    arrayLength_ = this->computeArrayLength(length);

    try {
      tritSet_ = new uint[arrayLength_];
      this->initArray(tritSet_, arrayLength_);
    } catch (bad_alloc& e) {
      std::cout << "Cannot allocate array of size " << arrayLength_ << std::endl;
      throw;
    }
  }

  TritSet::TritHolder::
  ~TritHolder()
  {
    if (tritSet_!= nullptr)
      delete[] tritSet_;
    tritSet_ = nullptr;
  }

  uint TritSet::TritHolder::
  getPlaceholder() {
    uint placeholder = 0xAAu;
    for (size_t i = 1; i < sizeof(uint); ++i) {
      placeholder = (placeholder << 8) + 0xAAu;
    }
    return placeholder;
  }

  void TritSet::TritHolder::
  initArray(uint* arr, const size_t length)
  {
    uint placeholder = getPlaceholder();
    for (size_t i = 0; i < length; ++i) {
      arr[i] = placeholder;
    }
  }

  size_t TritSet::TritHolder::
  computeArrayLength(const size_t length)
  {
    size_t integer = (length * 2) / (8 * sizeof(uint));
    size_t remainder = (length * 2) % (8 * sizeof(uint));

    return integer + (remainder > 0 ? 1 : 0);
  }

  void TritSet::TritHolder::
  cleanAfter(const size_t index)
  {
    if (tritSetLength_ == 0) {
      return;
    }

    uint placeholder = getPlaceholder();
    uint trigger = (1u << (2*index)) - 1u;
    tritSet_[arrayLength_-1] &= ~trigger; // clean after `index` trit (inclusive)
    tritSet_[arrayLength_-1] |= (placeholder & trigger); // set cleaned bits to placeholder value

  }

  void TritSet::TritHolder::
  findLastSet()
  {
    uint placeholder = getPlaceholder();
    size_t nonDefaultInd = 0;
    for (size_t i = 0; i < arrayLength_; ++i) {
      if (tritSet_[arrayLength_-1-i] != placeholder) {
        nonDefaultInd = arrayLength_-1-i;
      }
    }

    lastSet_ = 0;
    for (size_t i = 0; i < 8*sizeof(uint); i+=2) {
      if (((tritSet_[nonDefaultInd] >> i) & 3) != 2) {
        lastSet_ = (i + nonDefaultInd * sizeof(uint) * 8) / 2;
      }
    }
  }

  void TritSet::TritHolder::
  resize(const size_t length) // length === number of trits
  {
    std::cout << "resizing length: " << length << std::endl;

    size_t tempArrayLength = this->computeArrayLength(length);
    if (tempArrayLength == arrayLength_) {
      return;
    }

    uint* tempTritSet = nullptr;
    try {
      std::cout << "Trying resize from " << arrayLength_ << " to " << tempArrayLength << std::endl;

      tempTritSet = new uint[tempArrayLength];
      this->initArray(tempTritSet, tempArrayLength);

      size_t workLength = (arrayLength_ < tempArrayLength? arrayLength_ : tempArrayLength);
      memcpy(tempTritSet, tritSet_, sizeof(uint) * workLength);

      delete[] tritSet_;
      tritSet_ = tempTritSet;

      arrayLength_ = tempArrayLength;
      tritSetLength_ = length;
    } catch (bad_alloc& e) {
      std::cout << "Cannot resize tritSet. Keeping on working with the old \
                    one of size " << arrayLength_ << std::endl;
      throw ("hello");
    }
  }

  Trit& TritSet::TritHolder::
  operator= (uint tritPrototype)
  {
    Trit trit = Unknown;
    Trit& tritref = trit;

    if (tritPrototype < False || tritPrototype > True) {
      throw std::out_of_range("TritSet::TritHolder::operator=: "
                                      "Trit must be inside range [0, 3], "
                                      "but was assigned value of " + std::to_string(tritPrototype) + ".");
    }

    trit = static_cast<Trit>(tritPrototype);

    if (index_ > lastSet_) {
      if (trit == Unknown) return tritref;
      if (index_ >= tritSetLength_) resize(index_ + 1);
      lastSet_ = index_;
    }

    uint setter = static_cast<uint>(trit);

    size_t arrayIndex = index_ / (sizeof(uint) * 4);
    size_t tritIndex = 2*(index_ % (sizeof(uint) * 4));

    tritSet_[arrayIndex] &= ~(3 << tritIndex); // clean bits
    tritSet_[arrayIndex] |= ((setter) << tritIndex); // set bits

    if (trit == Unknown && index_ == lastSet_) {
      findLastSet();
    }

    return tritref;
  }

  TritSet::TritHolder::
  operator Trit()
  {
    if (index_ >= tritSetLength_) {
      return Unknown;
    }

    size_t arrayIndex = index_ / (sizeof(uint) * 4);
    size_t tritIndex = 2*(index_ % (sizeof(uint) * 4));

    return static_cast<Trit>((tritSet_[arrayIndex] >> tritIndex) & 3);
  }

  bool TritSet::TritHolder::
  operator==(Trit trit)
  {
    return this->equals(trit);
  }

  bool TritSet::TritHolder::
  operator!=(Trit trit)
  {
    return !this->equals(trit);
  }

  Trit TritSet::TritHolder::
  operator!()
  {
    if (index_ > lastSet_)
      return Unknown;

    size_t arrayIndex = index_ / (sizeof(uint) * 4);
    size_t tritIndex = 2*(index_ % (sizeof(uint) * 4));

    return (Trit) !static_cast<Trit>((tritSet_[arrayIndex] >> tritIndex) & 3);
  }

  bool TritSet::TritHolder::
  equals(Trit trit) {

    if (trit < False || trit > True) {
      return false;
//      throw std::out_of_range("TritHolder::equals: "
//                                "Attempt to compare Trit with inappropriate value: " + std::to_string((uint)trit));
    }
    if (index_ > lastSet_) {
      return trit == Unknown;
    }
    size_t arrayIndex = index_ / (sizeof(uint) * 4);
    size_t tritIndex = 2*(index_ % (sizeof(uint) * 4));
    uint innerTrit = (tritSet_[arrayIndex] >> tritIndex) & 3;

    return (trit + 2 >= innerTrit);
  }

  Trit&
  operator!(Trit t)
  {
    Trit& tr = t;
    switch (t) {
      case True:
        tr = False;
        break;
      case False:
        tr = True;
        break;
      default:
        tr = Unknown;
    }

    return tr;
  }

}
