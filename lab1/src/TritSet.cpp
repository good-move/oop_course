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
    for (auto& it : tritSet) {
      os << it << " ";
    }
    os << std::endl;
    return os;
  }

  TritSet::
  TritSet()
  {
    tritSet_ = nullptr;
    allocLength_ = 0;
    currentLength_ = 0;
    maxSetInd_ = 0;
  }

  TritSet::
  TritSet(const size_t length)
  {
    arrayLength_ = this->computeArrayLength(length);
    tritSet_ = new uint[arrayLength_];
    allocLength_ = length;
    currentLength_ = length;
    maxSetInd_ = 0;

    this->initArray(tritSet_, arrayLength_);
  }

  TritSet:: // copy constructor
  TritSet(const TritSet &outerSet)
  {
    tritSet_ = new uint[outerSet.capacity()];
    std::copy(outerSet.tritSet_, outerSet.tritSet_ + outerSet.capacity(), tritSet_);

    allocLength_ = outerSet.allocLength_;
    currentLength_ = outerSet.currentLength_;
    arrayLength_ = outerSet.arrayLength_;
    maxSetInd_ = outerSet.maxSetInd_;
  }

  TritSet:: // move contructor
  TritSet(TritSet &&outerSet)
  {
    tritSet_ = outerSet.tritSet_;
    outerSet.tritSet_ = nullptr;

    this->copyStateFrom(outerSet);
  }

  TritSet::
  ~TritSet()
  {
    delete[] tritSet_;
    tritSet_ = nullptr;
  }

  size_t TritSet::
  cardinality(Trit trit) const
  {
    size_t tritCount = 0;

    for (size_t i = 0; i < this->length(); ++i) {
      if ((*this)[i] == trit) {
        ++tritCount;
      }
    }

    return tritCount;
  }

  unordered_map<Trit, size_t, TritHash> TritSet::
  cardinality() const
  {
    unordered_map<Trit, size_t, TritHash> map;

    for (size_t i = 0; i < this->length(); ++i) {
      Trit trit = (*this)[i];
      map[trit]++;
    }

    return map;
  }

  size_t TritSet::
  capacity() const
  {
    return arrayLength_;
  }

  size_t TritSet::
  length() const
  {
    if (maxSetInd_ == 0 &&
        static_cast<Trit>((*this)[0]) == Unknown)
      return 0;
    else
      return maxSetInd_ + 1;
  }

  void TritSet::
  shrink()
  {
    size_t minSize = this->length() < allocLength_?
                     this->length() : allocLength_;
    this->trim(minSize);
  }

  void TritSet::
  trim(const size_t lastIndex)
  {
    if (lastIndex >= 0 && lastIndex < currentLength_) {
      this->resize(lastIndex);
      this->cleanAfter(lastIndex);
      this->findMaxSetInd();
    }
  }

  TritSet::TritHolder TritSet::
  operator[](const size_t index)
  {
    return TritHolder(*this, index);
  }

  TritSet::ConstTritHolder TritSet::
  operator[](const size_t index) const
  {
    return ConstTritHolder(*this, index);
  }

  TritSet& TritSet::
  operator=(const TritSet &outerSet)
  {
    if (this != &outerSet) {
      uint* tempTritSet = new uint[outerSet.capacity()];
      std::copy(outerSet.tritSet_, outerSet.tritSet_ + outerSet.capacity(), tempTritSet);

      if (tritSet_ != nullptr)
        delete[] tritSet_;
      tritSet_ = tempTritSet;
      this->copyStateFrom(outerSet);
    }

    return *this;
  }

  TritSet& TritSet::
  operator&=(const TritSet &outerSet)
  {
    this->logicOperation(outerSet, TritSet::conjunction);

    return *this;
  }

  TritSet TritSet::
  operator&(const TritSet &outerSet) const
  {
    TritSet tmp(*this);
    tmp &= outerSet;
    return tmp;
  }

  TritSet& TritSet::
  operator|=(const TritSet &outerSet)
  {
    this->logicOperation(outerSet, TritSet::disjunction);

    return *this;
  }

  TritSet TritSet::
  operator|(const TritSet &outerSet) const
  {
    TritSet tmp(*this);
    tmp |= outerSet;
    return tmp;
  }

  bool TritSet::
  operator==(const TritSet &outerSet) const
  {
    return this->equals(outerSet);
  }

  bool TritSet::
  operator!=(const TritSet &outerSet) const
  {
    return !this->equals(outerSet);
  }

  TritSet TritSet::
  operator~()
  {
    TritSet tmp(*this);
    for (size_t i = 0; i < tmp.length(); ++i)
      (tmp)[i] = !((tmp)[i]);

    return tmp;
  }

  bool TritSet::
  equals(const TritSet &outerSet) const
  {
    if (this->length() != outerSet.length())
      return false;

    for (size_t i = 0; i < this->capacity(); ++i) {
      if (tritSet_[i] != outerSet.tritSet_[i]) {
        return false;
      }
    }

    return true;
  }

  void TritSet::
  logicOperation(const TritSet &outerSet, uint(*logicOperator)(uint, uint))
  {
    uint placeholder = TritSet::getPlaceholder();
    size_t maxCapacity = this->capacity() > outerSet.capacity()?
                         this->capacity() : outerSet.capacity();

    this->resize(this->capacityToLength(maxCapacity));

    for (size_t i = 0; i < maxCapacity; ++i) {
      if (i < outerSet.capacity()) {
        tritSet_[i] = logicOperator(tritSet_[i], outerSet.tritSet_[i]);
      } else {
        tritSet_[i] = logicOperator(tritSet_[i], placeholder); // implies tritSet_[i] &= (|=) Unknown[]
      }
    }

    this->findMaxSetInd();
  }

  void TritSet::
  copyStateFrom(const TritSet& outerTh)
  {
    allocLength_ = outerTh.allocLength_;
    currentLength_ = outerTh.currentLength_;
    arrayLength_ = outerTh.arrayLength_;
    maxSetInd_ = outerTh.maxSetInd_;
  }

  uint TritSet::
  getPlaceholder()
  {
    uint placeholder = 0xAAu;
    for (size_t i = 1; i < sizeof(uint); ++i) {
      placeholder = (placeholder << 8) + 0xAAu;
    }
    return placeholder;
  }

  void TritSet::
  initArray(uint *arr, const size_t length) // fill tritHolder with `Unknown` values
  {
    uint placeholder = TritSet::getPlaceholder();
    for (size_t i = 0; i < length; ++i) {
      arr[i] = placeholder;
    }
  }

  size_t TritSet::
  computeArrayLength(const size_t length) const
  {
    size_t integer = this->countUintIndex(length);
    size_t remainder = this->countOffset(length);

    return integer + (remainder > 0 ? 1 : 0);
  }

  void TritSet::
  cleanAfter(const size_t index)
  {
    if (currentLength_ == 0) {
      return;
    }
    uint placeholder = TritSet::getPlaceholder();
    uint trigger = (1u << (BITS_PER_TRIT * index)) - 1u;
    tritSet_[arrayLength_ - 1] &= trigger; // clean after `index` trit
    tritSet_[arrayLength_ - 1] |= (placeholder & ~trigger); // set cleaned bits to placeholder value
  }

  void TritSet::
  findMaxSetInd()
  {
    uint placeholder = TritSet::getPlaceholder();
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
        maxSetInd_ = this->capacityToLength(nonEmptyByteIndex) + offset / BITS_PER_TRIT;
      }
    }
  }

  void TritSet::
  resize(const size_t numOfTrits)
  {
    size_t tempArrayLength = this->computeArrayLength(numOfTrits);
    if (tempArrayLength == arrayLength_) {
      return;
    }

    uint *tempTritSet = new uint[tempArrayLength];
    this->initArray(tempTritSet, tempArrayLength);

    size_t workLength = (arrayLength_ < tempArrayLength ? arrayLength_ : tempArrayLength);
    std::copy(tritSet_, tritSet_ + workLength, tempTritSet);

    delete[] tritSet_;
    tritSet_ = tempTritSet;

    arrayLength_ = tempArrayLength;
    currentLength_ = numOfTrits;
  }

  Trit TritSet::
  getTrit(size_t index) const
  {
    if (index > maxSetInd_ || currentLength_ == 0 || tritSet_ == nullptr) {
      return Unknown;
    }

    size_t uintIndex = this->countUintIndex(index);
    size_t tritOffset = this->countOffset(index);

    uint innerTrit = (tritSet_[uintIndex] >> tritOffset) & TRIT_STAMP;

    return static_cast<Trit>(innerTrit);
  }

  Trit TritSet::
  getTrit(size_t uintIndex, size_t offset) const
  {
    uint innerTrit = (tritSet_[uintIndex] >> offset) & TRIT_STAMP;

    return static_cast<Trit>(innerTrit);
  }

  void TritSet::
  setTrit(size_t index, Trit trit)
  {
    if (index > maxSetInd_) {
      if (trit == Unknown) {
        return;
      }
      if (index >= currentLength_) {
        this->resize(index + 1);
      }
      maxSetInd_ = index;
    }

    uint setter = static_cast<uint>(trit);

    size_t uintIndex = this->countUintIndex(index);
    size_t tritOffset = this->countOffset(index);

    tritSet_[uintIndex] &= ~(TRIT_STAMP << tritOffset); // clean bits
    tritSet_[uintIndex] |= ((setter) << tritOffset); // set bits

    if (trit == Unknown && index == maxSetInd_) {
      this->findMaxSetInd();
    }
  }

  TritSet::iterator TritSet::
  begin()
  {
    return TritSet::iterator(this, 0);
  }

  TritSet::iterator TritSet::
  end()
  {
    return TritSet::iterator(this, this->length());
  }

  TritSet::const_iterator TritSet::
  begin() const
  {
    return TritSet::const_iterator(this, 0);
  }

  TritSet::const_iterator TritSet::
  end() const
  {
    return TritSet::const_iterator(this, this->length());
  }

  size_t  TritSet::
  countUintIndex(size_t index) const
  {
    return index * BITS_PER_TRIT / BITS_PER_UINT;
  }

  size_t  TritSet::
  countOffset(size_t index) const
  {
    return (index * BITS_PER_TRIT) % BITS_PER_UINT;
  }

  size_t  TritSet::
  capacityToLength(size_t capacity) const
  {
    return (capacity * BITS_PER_UINT) / BITS_PER_TRIT;
  }


  /* ************************* TritHolder functions ************************* */

  TritSet::TritHolder::
  TritHolder(TritSet& parent, size_t tritIndex) : parent_(parent)
  {
    tritIndex_ = tritIndex;
  }

  TritSet::TritHolder::
  TritHolder(const TritHolder &outerTh) : parent_(outerTh.parent_)
  {
    tritIndex_ = outerTh.tritIndex_;
  }

  TritSet::TritHolder::
  TritHolder(TritHolder &&outerTh) : parent_(outerTh.parent_)
  {
    tritIndex_ = outerTh.tritIndex_;
  }

  TritSet::TritHolder& TritSet::TritHolder::
  operator=(Trit trit)
  {
    parent_.setTrit(tritIndex_, trit);

    return *this;
  }

  TritSet::TritHolder& TritSet::TritHolder::
  operator=(TritHolder th)
  {
    parent_.setTrit(tritIndex_, static_cast<Trit>(th));

    return *this;
  }

  TritSet::TritHolder::
  operator Trit() const
  {
    return parent_.getTrit(tritIndex_);
  }

  bool TritSet::TritHolder::
  operator==(Trit trit) const
  {
    return this->equals(trit);
  }

  bool TritSet::TritHolder::
  operator!=(Trit trit) const
  {
    return !this->equals(trit);
  }

  Trit TritSet::TritHolder::
  operator!() const
  {
    return tritIndex_ > parent_.maxSetInd_?
           Unknown : static_cast<Trit>(!parent_.getTrit(tritIndex_));
  }

  size_t TritSet::TritHolder::
  getIndex() const
  {
    return tritIndex_;
  }

  void TritSet::TritHolder::
  setIndex(size_t ind)
  {
    tritIndex_ = ind;
  }

  bool TritSet::TritHolder::
  equals(Trit trit) const
  {
    return parent_.getTrit(tritIndex_) == trit;
  }

/* ************************* ConstTritHolder functions ************************* */

  TritSet::ConstTritHolder::
  ConstTritHolder(const TritSet& parent, size_t tritIndex) : parent_(parent)
  {
    tritIndex_ = tritIndex;
  }

  TritSet::ConstTritHolder::
  ConstTritHolder(const ConstTritHolder &outerTh) : parent_(outerTh.parent_)
  {
    tritIndex_ = outerTh.tritIndex_;
  }

  TritSet::ConstTritHolder::
  ConstTritHolder(ConstTritHolder &&outerTh) : parent_(outerTh.parent_)
  {
    tritIndex_ = outerTh.tritIndex_;
  }

  TritSet::ConstTritHolder::
  operator Trit() const
  {
    return parent_.getTrit(tritIndex_);
  }

  bool TritSet::ConstTritHolder::
  operator==(Trit trit) const
  {
    return this->equals(trit);
  }

  bool TritSet::ConstTritHolder::
  operator!=(Trit trit) const
  {
    return !this->equals(trit);
  }

  Trit TritSet::ConstTritHolder::
  operator!() const
  {
    return tritIndex_ > parent_.maxSetInd_?
           Unknown : (Trit)!parent_.getTrit(tritIndex_);
  }

  size_t TritSet::ConstTritHolder::
  getIndex() const
  {
    return tritIndex_;
  }

  void TritSet::ConstTritHolder::
  setIndex(size_t ind)
  {
    tritIndex_ = ind;
  }

  bool TritSet::ConstTritHolder::
  equals(Trit trit) const
  {
    return parent_.getTrit(tritIndex_) == trit;
  }

}