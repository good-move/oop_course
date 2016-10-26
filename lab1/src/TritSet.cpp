#include "TritSet.h"

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
    tritHolder_->lastAccessedInd_               = outerTh->lastAccessedInd_;
    tritHolder_->allocTritSetLength_  = outerTh->allocTritSetLength_;
    tritHolder_->tritSetLength_       = outerTh->tritSetLength_;
    tritHolder_->arrayLength_         = outerTh->arrayLength_;
    tritHolder_->maxSetInd_             = outerTh->maxSetInd_;
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
    tritHolder_->lastAccessedInd_ = index;

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
    if (this->capacity() > 0)
      return tritHolder_->maxSetInd_ + 1;
    else
      return 0;
  }

  void TritSet::
  shrink()
  {
    size_t minSize = 0;
    if (tritHolder_->maxSetInd_+1 < tritHolder_->allocTritSetLength_) {
      minSize = tritHolder_->maxSetInd_+1;
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
    uint placeholder = tritHolder_->getPlaceholder();
    size_t maxCapacity = (this->capacity() > outerSet.capacity()? this->capacity() : outerSet.capacity());
    tritHolder_->resize(maxCapacity * BITS_PER_UINT / BITS_PER_TRIT);

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        tritHolder_->tritSet_[i] &= outerSet.tritHolder_->tritSet_[i];
      } else {
        tritHolder_->tritSet_[i] &= placeholder; // implies ...[i] &= Unknown
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

    uint placeholder = tritHolder_->getPlaceholder();
    size_t maxCapacity = (this->capacity() > outerSet.capacity()? this->capacity() : outerSet.capacity());
    tritHolder_->resize(maxCapacity * BITS_PER_UINT / BITS_PER_TRIT);

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        tritHolder_->tritSet_[i] |= outerSet.tritHolder_->tritSet_[i];
      } else {
        tritHolder_->tritSet_[i] |= placeholder; // implies ...[i] |= Unknown
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

    uint* innerTritSet = tritHolder_->tritSet_;
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
    arrayLength_ = this->computeArrayLength(length);

    try {
      tritSet_ = new uint[arrayLength_];

      this->initArray(tritSet_, arrayLength_);
      allocTritSetLength_ = length;
      tritSetLength_ = length;
      maxSetInd_ = 0;
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

    if (lastAccessedInd_ > maxSetInd_) {
      if (trit == Unknown) {
        return tritref;
      }
      if (lastAccessedInd_ >= tritSetLength_) {
        this->resize(lastAccessedInd_ + 1);
      }
      maxSetInd_ = lastAccessedInd_;
    }

    uint setter = static_cast<uint>(trit);
    this->setTrit(setter);

    if (trit == Unknown && lastAccessedInd_ == maxSetInd_) {
      this->findLastSet();
    }

    return tritref;
  }

  TritSet::TritHolder::
  operator Trit()
  {
    if (lastAccessedInd_ >= maxSetInd_) {
      return Unknown;
    }

    return this->getTrit();
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
    if (lastAccessedInd_ > maxSetInd_)
      return Unknown;

    return (Trit) !this->getTrit();
  }

  bool TritSet::TritHolder::
  equals(Trit trit) {

    if (lastAccessedInd_ > maxSetInd_) {
      return trit == Unknown;
    }

    return this->getTrit() == trit;
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
    uint placeholder = this->getPlaceholder();
    for (size_t i = 0; i < length; ++i) {
      arr[i] = placeholder;
    }
  }

  size_t TritSet::TritHolder::
  computeArrayLength(const size_t length)
  {
    size_t integer = (length * BITS_PER_TRIT) / BITS_PER_UINT;
    size_t remainder = (length * BITS_PER_TRIT) % BITS_PER_UINT;

    return integer + (remainder > 0 ? 1 : 0);
  }

  void TritSet::TritHolder::
  cleanAfter(const size_t index)
  {
    if (tritSetLength_ == 0) {
      return;
    }

    uint placeholder = this->getPlaceholder();
    uint trigger = (1u << (BITS_PER_TRIT*index)) - 1u;
    tritSet_[arrayLength_-1] &= ~trigger; // clean after `index` trit (inclusive)
    tritSet_[arrayLength_-1] |= (placeholder & trigger); // set cleaned bits to placeholder value

  }

  void TritSet::TritHolder::
  findLastSet()
  {
    uint placeholder = this->getPlaceholder();
    size_t nonEmptyByteIndex = 0;

    for (size_t i = 0; i < arrayLength_; ++i) {
      if (tritSet_[arrayLength_-1-i] != placeholder) {
        nonEmptyByteIndex = arrayLength_-1-i;
      }
    }

    maxSetInd_ = 0;
    for (size_t offset = 0; offset < BITS_PER_UINT; offset+=2) {
      if (this->getTrit(nonEmptyByteIndex, offset) != Unknown) {
        maxSetInd_ = nonEmptyByteIndex * BITS_PER_UINT / BITS_PER_TRIT + offset;
      }
    }
  }

  void TritSet::TritHolder::
  resize(const size_t numOfTrits)
  {
    size_t tempArrayLength = this->computeArrayLength(numOfTrits);
    if (tempArrayLength == arrayLength_) {
      return;
    }

    uint* tempTritSet = nullptr;
    try {
      tempTritSet = new uint[tempArrayLength];
      this->initArray(tempTritSet, tempArrayLength);

      size_t workLength = (arrayLength_ < tempArrayLength? arrayLength_ : tempArrayLength);
      memcpy(tempTritSet, tritSet_, sizeof(uint) * workLength);

      delete[] tritSet_;
      tritSet_ = tempTritSet;

      arrayLength_ = tempArrayLength;
      tritSetLength_ = numOfTrits;
    } catch (bad_alloc& e) {
      std::cout << "Cannot resize tritSet. Keeping on working with the old \
                  one of size " << arrayLength_ << std::endl;
      throw;
    }
  }

  Trit TritSet::TritHolder::
  getTrit()
  {
    size_t uintIndex = lastAccessedInd_ / (BITS_PER_UINT / BITS_PER_TRIT);
    size_t tritOffset = BITS_PER_TRIT * (lastAccessedInd_ % (BITS_PER_UINT / BITS_PER_TRIT));

    uint innerTrit = (tritSet_[uintIndex] >> tritOffset) & TRIT_STAMP;

    return static_cast<Trit>(innerTrit);
  }

  Trit TritSet::TritHolder::
  getTrit(size_t index, size_t offset)
  {
    uint innerTrit = (tritSet_[index] >> offset) & TRIT_STAMP;

    return static_cast<Trit>(innerTrit);
  }

  void TritSet::TritHolder::
  setTrit(uint setter)
  {
    size_t uintIndex = lastAccessedInd_ / (BITS_PER_UINT / BITS_PER_TRIT);
    size_t tritOffset = BITS_PER_TRIT * (lastAccessedInd_ % (BITS_PER_UINT / BITS_PER_TRIT));

    tritSet_[uintIndex] &= ~(TRIT_STAMP << tritOffset); // clean bits
    tritSet_[uintIndex] |= ((setter) << tritOffset); // set bits
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
