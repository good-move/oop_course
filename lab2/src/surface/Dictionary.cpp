#include "../../include/surface/Dictionary.h"

namespace explorer {

  Dictionary::
  Dictionary()
  {
    isBuilt_ = false;
  }

//  Levenshtein distance
  size_t
  Dictionary::
  distance(const string& s1, const string& s2) const
  {
    const std::size_t len1 = s1.size(), len2 = s2.size();
    std::vector<size_t> col(len2+1), prevCol(len2+1);

    for (size_t i = 0; i < prevCol.size(); i++)
      prevCol[i] = i;
    for (size_t i = 0; i < len1; i++) {
      col[0] = i+1;
      for (size_t j = 0; j < len2; j++)
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
  checkPath(vector<string> path, string start, string finish) const
  {
    if (!isWordValid(start) || !isWordValid(finish)) {
      cout << "Start or finish word is invalid" << endl;
      cout << start << " " << finish << endl;
      return false;
    }
    if (path.size() == 0) {
      cout << "No path provided" << endl;
      return false;
    }
    if (path.front() != start || path.back() != finish) {
      cout << "Error: start(finish) argument doesn't match path start(finish)" << endl;
      return false;
    }

    string current = start;
    for (const auto &word : path) {
      if (!isWordValid(word)) {
        cout << "The word" << word << " is outside the surface bounds or is an obstacle." << endl;
        return false;
      }
      if (this->distance(current, word) > 1) {
        cout << "Error: can't jump over words" << endl;
        return false;
      }
      current = word;
    }

    return true;
  }

  vector<string>
  Dictionary::
  lookup(string str) const
  {
    vector<string> neighborhood;

    for (const auto& i : {-1, 0, 1}) {
      try {
        for (const string& word : dictionary_.at(str.length() + i)) {
          if (this->distance(word, str) == 1) {
            neighborhood.push_back(word);
          }
        }
      } catch (const out_of_range& e) {
        continue;
      }
    }

    return neighborhood;
  }

  unordered_map<size_t, vector<string>>
  Dictionary::getSurface() const
  {
    return dictionary_;
  }

  void
  Dictionary::
  setSurface(const unordered_map<size_t, vector<string>>& dictionary)
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
  isWalkable(string word) const
  {
    return this->isInDict(word);
  }

  bool
  Dictionary::
  isWordValid(string word) const
  {
    return this->isInDict(word);
  }

  bool
  Dictionary::
  isInDict(string str) const
  {
    try {
      for (string word : dictionary_.at(str.length())) {
        if (word == str) {
          return true;
        }
      }
    } catch (const out_of_range& e){}

    return false;
  }

}
