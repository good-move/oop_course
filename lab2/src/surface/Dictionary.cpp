#include "../../include/surface/Dictionary.h"

using namespace std;

namespace explorer {

  Dictionary::
  Dictionary()
  {
    isBuilt_ = false;
  }

//  Levenshtein distance
  Dictionary::measure_type
  Dictionary::
  distance(const point_type& s1, const point_type& s2) const
  {
    const measure_type len1 = s1.size(), len2 = s2.size();
    vector<measure_type> col(len2+1), prevCol(len2+1);

    for (measure_type i = 0; i < prevCol.size(); i++)
      prevCol[i] = i;

    for (measure_type i = 0; i < len1; i++) {
      col[0] = i+1;
      for (measure_type j = 0; j < len2; j++)
        col[j+1] = std::min(
                std::min(prevCol[1 + j] + 1, col[j] + 1),
                prevCol[j] + (s1[i]==s2[j] ? 0 : 1)
        );
      col.swap(prevCol);
    }
    return prevCol[len2];
  }

  bool
  Dictionary::
  checkPath(const point_vector& path, const point_type& start, const point_type& finish) const
  {
    if (!isWordValid(start) || !isWordValid(finish)) {
      throw invalid_argument("Start or finish word is invalid.");
    }
    if (path.size() == 0) {
      throw invalid_argument("No path provided");
    }
    if (path.front() != start || path.back() != finish) {
      throw logic_error("Error: start(finish) argument doesn't match path start(finish)");
    }

    point_type current = start;
    for (const auto &word : path) {
      if (!isWordValid(word)) {
        throw logic_error("The word " + word + " is outside the surface bounds "
                           "or is an obstacle.");
      }
      if (this->distance(current, word) > 1) {
        throw logic_error("Error: can't jump over words");
      }
      current = word;
    }

    return true;
  }

  Dictionary::point_vector
  Dictionary::
  lookup(const point_type& str) const
  {
    point_vector neighborhood;

    for (const auto& i : {-1, 0, 1}) {
      if (dictionary_.count(str.length() + i) > 0) {
        for (const point_type& word : dictionary_.at(str.length() + i)) {
          if (this->distance(word, str) == 1) {
            neighborhood.push_back(word);
          }
        }
      }
    }

    return neighborhood;
  }

  Dictionary::surface_points
  Dictionary::getSurface() const
  {
    return dictionary_;
  }

  void
  Dictionary::
  setSurface(const surface_points& dictionary)
  {
    if (!isBuilt_) {
      dictionary_ = dictionary;
      isBuilt_ = true;
    }
  }

  bool
  Dictionary::
  isBuilt() const
  {
    return isBuilt_;
  }

  bool
  Dictionary::
  isWalkable(const point_type& word) const
  {
    return this->isInDict(word);
  }

  bool
  Dictionary::
  isWordValid(const point_type& word) const
  {
    return this->isInDict(word);
  }

  bool
  Dictionary::
  isInDict(const point_type& str) const
  {
    try {
      for (point_type word : dictionary_.at(str.length())) {
        if (word == str) {
          return true;
        }
      }
    } catch (const out_of_range& e){}

    return false;
  }

}
