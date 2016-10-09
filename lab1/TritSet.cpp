#include "TritSet.hpp"

namespace alexgm {

  /* **************************  TritSet functions ************************** */

  TritSet::
  TritSet(const size_t length)
  {
    tritHolder_ = new TritHolder(length);
  }

  TritSet::
  TritSet(const TritSet& tritSet)
  {
    tritHolder_ = new TritHolder(tritSet.length());

    TritHolder* outerTh = tritSet.tritHolder_;
    tritHolder_->index_               = outerTh->index_;
    tritHolder_->allocTritSetLength_  = outerTh->allocTritSetLength_;
    tritHolder_->tritSetLength_       = outerTh->tritSetLength_;
    tritHolder_->arrayLength_         = outerTh->arrayLength_;
    tritHolder_->lastSet_             = outerTh->lastSet_;
    memcpy(tritHolder_->tritSet_, outerTh->tritSet_, sizeof(uint) * outerTh->arrayLength_);
  }

  TritSet::
  ~TritSet()
  {
    delete tritHolder_;
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
    size_t minSize = (tritHolder_->lastSet_+1 < tritHolder_->allocTritSetLength_ ?
                      tritHolder_->lastSet_+1 : tritHolder_->allocTritSetLength_);
    this->trim(minSize);
  }

  void TritSet::
  trim(size_t lastIndex)
  {
    if (lastIndex >= 0 && lastIndex < tritHolder_->tritSetLength_) {
      tritHolder_->resize(lastIndex);
      tritHolder_->cleanAfter(lastIndex);
      tritHolder_->findLastSet();
    }
  }

  TritSet& TritSet::
  operator= (TritSet const& outerSet)
  {
    std::cout << "TritSet::inside operator=" << std::endl;
    if (this != &outerSet) {
      TritHolder* innerTh = this->tritHolder_;
      TritHolder* outerTh = outerSet.tritHolder_;

      delete[] innerTh->tritSet_;
      //copy the guts of outerSet
      innerTh->index_ = outerTh->index_;
      innerTh->allocTritSetLength_ = outerTh->allocTritSetLength_;
      innerTh->tritSetLength_ = outerTh->tritSetLength_;
      innerTh->arrayLength_ = outerTh->arrayLength_;
      innerTh->lastSet_ = outerTh->lastSet_;
      //copy tritSet
      try {
        innerTh->tritSet_ = new uint[innerTh->arrayLength_];
      } catch(bad_alloc& e) {
        std::cout << "Couldn't allocate " << innerTh->arrayLength_ <<
                  "bytes for TritSet copy"  << std::endl;
      }
      memcpy(innerTh->tritSet_, outerTh->tritSet_, sizeof(uint) * innerTh->arrayLength_);
    }
    return *this;
  }

  TritSet& TritSet::
  operator&= (TritSet const& outerSet)
  {
    size_t maxCapacity = 0;

    if (this->length() > outerSet.length()){
      maxCapacity = this->capacity();
    } else {
      maxCapacity = outerSet.capacity();
      this->tritHolder_->resize(outerSet.length());
    }

    for (size_t i = 0; i < maxCapacity; i++) {
      if (i < outerSet.capacity()) {
        this->tritHolder_->tritSet_[i] &= outerSet.tritHolder_->tritSet_[i];
      } else {
        this->tritHolder_->tritSet_[i] &= 2; // implies ...[i] &= Unknown
      }
    }

    this->tritHolder_->findLastSet();

    return *this;
  }

  TritSet TritSet::
  operator& (TritSet const& outerSet)
  {
    // std::cout << this->length() << std::endl; // <-- prevents segfault ??!!
    TritSet set(outerSet);
    set &= (*this);

    return set;
  }

  /* ************************* TritHolder functions ************************* */

  TritSet::TritHolder::
  TritHolder(const size_t length)
  {
    allocTritSetLength_ = length;
    tritSetLength_ = length;

    arrayLength_ = this->computeArrayLength(length);

    try {
      tritSet_ = new uint[arrayLength_];
      this->initArray(tritSet_, arrayLength_);
    } catch (bad_alloc& e) {
      std::cout << "Cannot allocate array of size " << arrayLength_ << std::endl;
    }
  }

  TritSet::TritHolder::
  ~TritHolder()
  {
    delete[] tritSet_;
  }

  void TritSet::TritHolder::
  initArray(uint arr[], const size_t length)
  {
    uint placeholder = 0xAA;
    for (size_t i = 1; i < sizeof(uint); i++) {
      placeholder = (placeholder << 8) + 0xAA;
    }
    for (size_t i = 0; i < length; i++) {
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

    // std::cout << "index:" << index << std::endl;
    // std::cout << "arrayLength_:" << arrayLength_ << std::endl;
    // for (size_t i = 1; i <= 8*sizeof(uint); i++) {
    //   uint t = (tritSet_[arrayLength_-1] >> (8*sizeof(uint) - i)) & 1;
    //   std::cout << t;
    // }

    for (size_t tritIndex = 2*index; tritIndex <= 8*sizeof(uint)-2; tritIndex+=2) {
      std::cout << "tritIndex:" << tritIndex << std::endl;
      tritSet_[arrayLength_-1] &= ~(3 << tritIndex); // clean bits
      tritSet_[arrayLength_-1] |= (2 << tritIndex); // set bits
    }

    // for (size_t i = 1; i <= 8*sizeof(uint); i++) {
    //   uint t = (tritSet_[arrayLength_-1] >> (8*sizeof(uint) - i)) & 1;
    //   std::cout << t;
    // }
  }

  void TritSet::TritHolder::
  findLastSet()
  {

  }

  void TritSet::TritHolder::
  resize(const size_t length)
  {
    size_t tempArrayLength = this->computeArrayLength(length);
    if (tempArrayLength == arrayLength_) {
      return;
    }
    uint* tempTritSet;

    try {
      std::cout << "Trying resize" << std::endl;
      tempTritSet = new uint[tempArrayLength];
      this->initArray(tempTritSet, tempArrayLength);
    } catch (bad_alloc& e) {
      std::cout << "Cannot resize tritSet. Keeping on working with the old \
                    one of size " << arrayLength_ << std::endl;
    }

    size_t workLength = (arrayLength_ < tempArrayLength ? arrayLength_ : tempArrayLength);
    memcpy(tempTritSet, tritSet_, sizeof(uint) * workLength);
    delete[] tritSet_;
    tritSet_ = tempTritSet;

    arrayLength_ = tempArrayLength;
    tritSetLength_ = length;
  }

  Trit& TritSet::TritHolder::
  operator= (Trit trit)
  {
    if (this->index_ >= this->tritSetLength_) {
      std::cout << "Special case for operator=" << std::endl;
      if (trit == Unknown) {
        Trit& tempTrit = trit;
        return tempTrit;
      } else {
        this->resize(this->index_ + 1);
        this->lastSet_ = this->index_;
        // return Unknown;
      }
    }
    else {
      if (this->lastSet_ < this->index_) {
        this->lastSet_ = this->index_;
      }
    }

    uint setter = 0;
    if (trit == -1) {
      setter = 0;
    } else if (trit == 1) {
      setter = 3;
    } else {
      setter = 2;
    }
    size_t arrayIndex = this->index_ / 4 / sizeof(uint);
    size_t tritIndex = 2*(this->index_ % (sizeof(uint) * 4));
    tritSet_[arrayIndex] &= ~(3 << tritIndex); // clean bits
    tritSet_[arrayIndex] |= ((setter) << tritIndex); // set bits

    Trit& tempTrit = trit;

    return tempTrit;
  }

  TritSet::TritHolder::
  operator Trit()
  {
    if (this->index_ >= this->tritSetLength_) {
      std::cout << "tritSetLength_:" << this->tritSetLength_ << std::endl;
      std::cout << "unset";
      return Unknown;
    }

    size_t arrayIndex = this->index_ / 4 / sizeof(uint);
    size_t tritIndex = 2*(this->index_ % (sizeof(uint) * 4));
    uint trit = (tritSet_[arrayIndex] >> tritIndex) & 3;

    switch (trit) {
      case 0:
        return False;
      case 1:
      case 2:
        return Unknown;
      case 3:
        // std::cout << "true";
        return True;
      default:
        // std::cout << "unset";
        return Unknown;
    }
  }

  bool TritSet::TritHolder::
  operator== (Trit trit)
  {
    if (this->index_ >= this->tritSetLength_) {
      return trit == Unknown;
    }
    size_t arrayIndex = this->index_ / 4 / sizeof(uint);
    size_t tritIndex = 2*(this->index_ % (sizeof(uint) * 4));
    uint innerTrit = (tritSet_[arrayIndex] >> tritIndex) & 3;

    if ((trit == -1 && innerTrit == 0) ||
        (trit == 0 && innerTrit == 2) ||
        (trit == 1 && innerTrit == 3)) {
      return true;
    } else {
      return false;
    }

  }

}
