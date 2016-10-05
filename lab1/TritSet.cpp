#include "TritSet.hpp"

namespace alexgm {

  /* ********************  TritSet functions ******************** */

  TritSet::
  TritSet(const size_t length)
  {
    tritHolder_ = new TritHolder(length);
  }

  TritSet::
  ~TritSet()
  {
    delete tritHolder_;
  }

  TritSet::TritHolder& TritSet::
  operator[](const size_t index)
  {
    tritHolder_->setCurrentIndex(index);

    return *tritHolder_;
  }

  /* ********************  TritHolder functions ******************** */

  TritSet::TritHolder::
  TritHolder(const size_t length)
  {
    allocTritSetLength_ = length;
    tritSetLength_ = length;

    arrayLength_ = this->computeArrayLength(length);

    try {
      tritSet_ = new uint[arrayLength_];

      for (size_t i = 0; i < arrayLength_; i++) {
        tritSet_[i] = ~tritSet_[i];
      }
    } catch (bad_alloc& e) {
      std::cout << "Cannot allocate array of size " << arrayLength_ << std::endl;
    }
  }

  TritSet::TritHolder::
  ~TritHolder()
  {
    delete[] tritSet_;
  }

  size_t TritSet::TritHolder::
  computeArrayLength(const size_t length)
  {
    size_t integer = (length * 2) / (8 * sizeof(uint));
    size_t remainder = (length * 2) % (8 * sizeof(uint));
    return integer + (remainder > 0 ? 1 : 0);
  }

  void TritSet::TritHolder::
  resize(const size_t length)
  {
    size_t tempArrayLength = this->computeArrayLength(length);

    uint* tempTritSet = new uint[tempArrayLength];
    for (size_t i = arrayLength_; i < tempArrayLength; i++) {
      tempTritSet[i] = ~tempTritSet[i];
    }
    memcpy(tempTritSet, tritSet_, sizeof(uint) * arrayLength_);
    delete[] tritSet_;
    tritSet_ = tempTritSet;

    arrayLength_ = tempArrayLength;
    tritSetLength_ = length;
  }

  void TritSet::TritHolder::
  setCurrentIndex(const size_t index)
  {
    this->index_ = index;
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
        try{
          std::cout << "Trying resize" << std::endl;
          this->resize(this->index_ + 1);
          lastSet_ = this->index_;
        } catch (bad_alloc& e) {
          std::cout << "Cannot resize tritSet. Keeping on working with the old \
                        one of size " << arrayLength_ << std::endl;
          // return Unknown;
        }
      }
    }

    size_t arrayIndex = this->index_ / 4 / sizeof(uint);
    size_t tritIndex = 2*(this->index_ % (sizeof(uint) * 4));
    tritSet_[arrayIndex] &= ~(3 << tritIndex); // clean bits
    tritSet_[arrayIndex] |= (trit << tritIndex); // set bits

    Trit& tempTrit = trit;
    return tempTrit;
  }

  TritSet::TritHolder::
  operator Trit()
  {
    if (this->index_ >= this->tritSetLength_) {
      std::cout << "unset" << std::endl;
      return Unknown;
    }

    size_t arrayIndex = this->index_ / 4 / sizeof(uint);
    size_t tritIndex = 2*(this->index_ % (sizeof(uint) * 4));
    uint trit = (tritSet_[arrayIndex] >> tritIndex) & 3;

    switch (trit) {
      case 0:
        return False;
      case 1:
        return Unknown;
      case 2:
        return True;
      default:
        std::cout << "unset" << std::endl;
        return Unknown;
    }
  }

}
