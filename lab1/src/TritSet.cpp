#include "TritSet.h"

namespace alexgm {

  Trit operator!(Trit t) {
    switch (t) {
      case True:
        return False;
      case False:
        return True;
      default:
        return Unknown;
    }
  }

/* **************************  TritSet functions ************************** */

  ostream& operator<<(ostream& os, const TritSet& tritSet)
  {
    for (auto it : tritSet) {
      os << it << " ";
    }
    os << std::endl;
    return os;
  }

  TritSet::
  TritSet() {
    tritHolder_ = new TritHolder(0);
  }

  TritSet::
  TritSet(const size_t length) {
    tritHolder_ = new TritHolder(length);
  }

  TritSet:: // copy constructor
  TritSet(const TritSet &outerSet) {
    try {
      tritHolder_ = new TritHolder(outerSet.capacity() * BITS_PER_UINT / BITS_PER_TRIT);
      TritHolder *outerTh = outerSet.tritHolder_;

      std::copy(outerTh->tritSet_, outerTh->tritSet_ + outerSet.capacity(), tritHolder_->tritSet_);
      tritHolder_->copyFrom(*outerTh);
    } catch (bad_alloc &e) {
      cout << e.what();
      throw;
    }
  }

  TritSet:: // move contructor
  TritSet(TritSet &&outerSet) {
    this->tritHolder_ = outerSet.tritHolder_;
    outerSet.tritHolder_ = nullptr;
  }

  TritSet::
  ~TritSet() {
    if (tritHolder_ != nullptr)
      delete tritHolder_;
    tritHolder_ = nullptr;
  }

  size_t TritSet::
  cardinality(Trit trit) const {
    size_t tritCount = 0;

    for (size_t i = 0; i < this->length(); ++i) {
      if ((*this)[i] == trit) {
        ++tritCount;
      }
    }

    return tritCount;
  }

  unordered_map<Trit, size_t, TritHash> TritSet::
  cardinality() const {
    unordered_map<Trit, size_t, TritHash> map;

    for (size_t i = 0; i < this->length(); ++i) {
      Trit trit = (*this)[i];
      map[trit]++;
    }

    return map;
  }

  size_t TritSet::
  capacity() const {
    return tritHolder_->arrayLength_;
  }

  size_t TritSet::
  length() const {
    if (tritHolder_->maxSetInd_ == 0 &&
        static_cast<Trit>((*this)[0]) == Unknown)
      return 0;
    else
      return tritHolder_->maxSetInd_ + 1;
  }

  void TritSet::
  shrink() {
    size_t minSize = this->length() < tritHolder_->allocTritSetLength_?
                     this->length() : tritHolder_->allocTritSetLength_;
    this->trim(minSize);
  }

  void TritSet::
  trim(const size_t lastIndex) {
    if (lastIndex >= 0 && lastIndex < tritHolder_->tritSetLength_) {
      tritHolder_->resize(lastIndex);
      tritHolder_->cleanAfter(lastIndex);
      tritHolder_->findMaxSetInd();
    }
  }

  TritSet::TritHolder &TritSet::
  operator[](const size_t index) const {
    tritHolder_->lastAccessedInd_ = index;
    return *tritHolder_;
  }

  TritSet &TritSet::
  operator=(const TritSet &outerSet) {
    if (this != &outerSet) {
      try {
        TritHolder *tmpTh = new TritHolder(outerSet.capacity() * BITS_PER_UINT / BITS_PER_TRIT);
        TritHolder *outerTh = outerSet.tritHolder_;

        tritHolder_->~TritHolder();
        delete tritHolder_;

        tritHolder_ = tmpTh;

        std::copy(outerTh->tritSet_, outerTh->tritSet_ + outerSet.capacity(), tritHolder_->tritSet_);
        tritHolder_->copyFrom(*outerTh);
      } catch (bad_alloc &e) {
        cout << e.what();
        throw;
      }
    }
    return *this;
  }

  TritSet &TritSet::
  operator&=(const TritSet &outerSet) {
    uint placeholder = tritHolder_->getPlaceholder();
    size_t maxCapacity = (this->capacity() > outerSet.capacity() ? this->capacity() : outerSet.capacity());
    tritHolder_->resize(maxCapacity * BITS_PER_UINT / BITS_PER_TRIT);

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        tritHolder_->tritSet_[i] &= outerSet.tritHolder_->tritSet_[i];
      } else {
        tritHolder_->tritSet_[i] &= placeholder; // implies ...[i] &= Unknown
      }
    }
    tritHolder_->findMaxSetInd();

    return *this;
  }

  TritSet TritSet::
  operator&(const TritSet &outerSet) const {
    TritSet tmp(*this);
    tmp &= outerSet;
    return tmp;
  }

  TritSet &TritSet::
  operator|=(const TritSet &outerSet) {
    uint placeholder = tritHolder_->getPlaceholder();
    size_t maxCapacity = (this->capacity() > outerSet.capacity() ? this->capacity() : outerSet.capacity());
    tritHolder_->resize(maxCapacity * BITS_PER_UINT / BITS_PER_TRIT);

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        tritHolder_->tritSet_[i] |= outerSet.tritHolder_->tritSet_[i];
      } else {
        tritHolder_->tritSet_[i] |= placeholder; // implies ...[i] |= Unknown
      }
    }
    tritHolder_->findMaxSetInd();

    return *this;
  }

  TritSet TritSet::
  operator|(const TritSet &outerSet) const {
    TritSet tmp(*this);
    tmp |= outerSet;
    return tmp;
  }

  bool TritSet::
  operator==(const TritSet &outerSet) const {
    return this->equals(outerSet);
  }

  bool TritSet::
  operator!=(const TritSet &outerSet) const {
    return !this->equals(outerSet);
  }

  TritSet TritSet::
  operator~() {
    TritSet tmp(*this);
    for (size_t i = 0; i < tmp.length(); ++i)
      (tmp)[i] = !((tmp)[i]);

    return tmp;
  }

  bool TritSet::
  equals(const TritSet &outerSet) const {
    if (this->capacity() != outerSet.capacity())
      return false;

    uint *innerTritSet = tritHolder_->tritSet_;
    uint *outerTritSet = outerSet.tritHolder_->tritSet_;

    for (size_t i = 0; i < this->capacity(); ++i) {
      if (innerTritSet[i] != outerTritSet[i]) {
        return false;
      }
    }
    return true;
  }

  TritSet::iterator TritSet::
  begin() {
    return TritSet::iterator(this, 0);
  }

  TritSet::iterator TritSet::
  end() {
    return TritSet::iterator(this, this->length());
  }

  TritSet::const_iterator TritSet::
  begin() const {
    return TritSet::const_iterator(this, 0);
  }

  TritSet::const_iterator TritSet::
  end() const {
    return TritSet::const_iterator(this, this->length());
  }

/* ************************* TritHolder functions ************************* */

  TritSet::TritHolder::
  TritHolder(const size_t length) {
    arrayLength_ = this->computeArrayLength(length);

    try {
      tritSet_ = new uint[arrayLength_];

      this->initArray(tritSet_, arrayLength_);
      allocTritSetLength_ = length;
      tritSetLength_ = length;
      maxSetInd_ = 0;
    } catch (bad_alloc &e) {
      std::cout << "Cannot allocate array of size " << arrayLength_ << std::endl;
      throw;
    }
  }

  TritSet::TritHolder::
  TritHolder(const TritHolder &outerTh) {
    try {
      tritSet_ = nullptr;
      uint *tritSet = new uint[outerTh.arrayLength_];
      std::copy(outerTh.tritSet_, outerTh.tritSet_ + outerTh.arrayLength_, tritSet);

      tritSet_ = tritSet;
      lastAccessedInd_ = outerTh.lastAccessedInd_;
      allocTritSetLength_ = outerTh.allocTritSetLength_;
      tritSetLength_ = outerTh.tritSetLength_;
      arrayLength_ = outerTh.arrayLength_;
      maxSetInd_ = outerTh.maxSetInd_;
    } catch (bad_alloc &e) {
      cout << e.what() << endl;
      throw;
    }
  }

  TritSet::TritHolder::
  TritHolder(TritHolder &&outerTh) {
    tritSet_ = outerTh.tritSet_;
    outerTh.tritSet_ = nullptr;

    this->copyFrom(outerTh);
  }

  TritSet::TritHolder::
  ~TritHolder() {
    if (tritSet_ != nullptr)
      delete[] tritSet_;
    tritSet_ = nullptr;
  }

  TritSet::TritHolder& TritSet::TritHolder::
  operator=(Trit trit) {
    return this->copyAssignment(trit);
  }

  TritSet::TritHolder& TritSet::TritHolder::
  operator=(const TritHolder outerTh) {
    return this->copyAssignment(static_cast<Trit>(outerTh));
  }

  TritSet::TritHolder::
  operator Trit() const {
    if (lastAccessedInd_ > maxSetInd_ || tritSetLength_ == 0) {
      return Unknown;
    }
    return this->getTrit();
  }

  bool TritSet::TritHolder::
  operator==(Trit trit) const {
    return this->equals(trit);
  }

  bool TritSet::TritHolder::
  operator!=(Trit trit) const {
    return !this->equals(trit);
  }

  Trit TritSet::TritHolder::
  operator!() {
    return lastAccessedInd_ > maxSetInd_? Unknown : (Trit) !this->getTrit();
  }

  bool TritSet::TritHolder::
  equals(Trit trit) const {
    if (lastAccessedInd_ > maxSetInd_) {
      return trit == Unknown;
    }
    return this->getTrit() == trit;
  }

  void TritSet::TritHolder::
  copyFrom(TritHolder outerTh)
  {
    lastAccessedInd_ = outerTh.lastAccessedInd_;
    allocTritSetLength_ = outerTh.allocTritSetLength_;
    tritSetLength_ = outerTh.tritSetLength_;
    arrayLength_ = outerTh.arrayLength_;
    maxSetInd_ = outerTh.maxSetInd_;
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
  initArray(uint *arr, const size_t length) { // fill tritHolder with `Unknown` values
    uint placeholder = this->getPlaceholder();
    for (size_t i = 0; i < length; ++i) {
      arr[i] = placeholder;
    }
  }

  size_t TritSet::TritHolder::
  computeArrayLength(const size_t length) {
    size_t integer = (length * BITS_PER_TRIT) / BITS_PER_UINT;
    size_t remainder = (length * BITS_PER_TRIT) % BITS_PER_UINT;

    return integer + (remainder > 0 ? 1 : 0);
  }

  void TritSet::TritHolder::
  cleanAfter(const size_t index) {
    if (tritSetLength_ == 0) {
      return;
    }
    uint placeholder = this->getPlaceholder();
    uint trigger = (1u << (BITS_PER_TRIT * index)) - 1u;
    tritSet_[arrayLength_ - 1] &= trigger; // clean after `index` trit
    tritSet_[arrayLength_ - 1] |= (placeholder & ~trigger); // set cleaned bits to placeholder value
  }

  void TritSet::TritHolder::
  findMaxSetInd() {
    uint placeholder = this->getPlaceholder();
    size_t nonEmptyByteIndex = 0;

    for (size_t i = 0; i < arrayLength_; ++i) {
      if (tritSet_[arrayLength_ - 1 - i] != placeholder) {
        nonEmptyByteIndex = arrayLength_ - 1 - i;
        break;
      }
    }
    maxSetInd_ = 0;
    for (size_t offset = 0; offset < BITS_PER_UINT; offset += BITS_PER_TRIT) {
      if (this->getTrit(nonEmptyByteIndex, offset) != Unknown) {
        maxSetInd_ = (nonEmptyByteIndex * BITS_PER_UINT + offset) / BITS_PER_TRIT;
      }
    }
  }

  void TritSet::TritHolder::
  resize(const size_t numOfTrits) {
    size_t tempArrayLength = this->computeArrayLength(numOfTrits);
    if (tempArrayLength == arrayLength_) {
      return;
    }

    try {
      uint *tempTritSet = new uint[tempArrayLength];
      this->initArray(tempTritSet, tempArrayLength);

      size_t workLength = (arrayLength_ < tempArrayLength ? arrayLength_ : tempArrayLength);
      std::copy(tritSet_, tritSet_ + workLength, tempTritSet);

      delete[] tritSet_;
      tritSet_ = tempTritSet;

      arrayLength_ = tempArrayLength;
      tritSetLength_ = numOfTrits;
    } catch (bad_alloc &e) {
      std::cout << "Cannot resize tritSet to tempArrayLength. Keeping on working with the old \
                one of size " << arrayLength_ << std::endl;
      throw;
    }
  }

  Trit TritSet::TritHolder::
  getTrit() const {
    size_t uintIndex = lastAccessedInd_ * BITS_PER_TRIT / BITS_PER_UINT;
    size_t tritOffset = (lastAccessedInd_ * BITS_PER_TRIT) % BITS_PER_UINT;

    uint innerTrit = (tritSet_[uintIndex] >> tritOffset) & TRIT_STAMP;

    return static_cast<Trit>(innerTrit);
  }

  Trit TritSet::TritHolder::
  getTrit(size_t index, size_t offset) const {
    uint innerTrit = (tritSet_[index] >> offset) & TRIT_STAMP;

    return static_cast<Trit>(innerTrit);
  }

  void TritSet::TritHolder::
  setTrit(uint setter) {
    size_t uintIndex = lastAccessedInd_ * BITS_PER_TRIT / BITS_PER_UINT;
    size_t tritOffset = (lastAccessedInd_ * BITS_PER_TRIT) % BITS_PER_UINT;

    tritSet_[uintIndex] &= ~(TRIT_STAMP << tritOffset); // clean bits
    tritSet_[uintIndex] |= ((setter) << tritOffset); // set bits
  }

  TritSet::TritHolder &TritSet::TritHolder::
  copyAssignment(Trit trit) {
    if (lastAccessedInd_ > maxSetInd_) {
      if (trit == Unknown) {
        return *this;
      }
      if (lastAccessedInd_ >= tritSetLength_) {
        this->resize(lastAccessedInd_ + 1);
      }
      maxSetInd_ = lastAccessedInd_;
    }

    uint setter = static_cast<uint>(trit);
    this->setTrit(setter);

    if (trit == Unknown && lastAccessedInd_ == maxSetInd_) {
      this->findMaxSetInd();
    }

    return *this;
  }

}