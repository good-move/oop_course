#include "../../include/serializer/DictionarySerializer.h"

namespace explorer {

  std::istream&
  DictionarySerializer::
  readSurface(istream& is, Dictionary& dictionary, bool lookForArgs) const
  {
    if (dictionary.isBuilt()) {
      return is;
    }

    unordered_map<size_t, vector<string>> surfaceRepr;

    // assuming the input dictionary has one word per line
    string word;

    if (lookForArgs) {
      if (!getline(is, word)) { // start word
        throw invalid_argument("Start point must be provided");
      }
      dictionary.start = word;

      if (!getline(is, word)) { // finish word
        throw invalid_argument("Finish point must be provided");
      }
      dictionary.finish = word;
    }

    while (getline(is, word)) {
      if (word.length() > 0) {
        surfaceRepr[word.length()].push_back(word);
      }
    }

    dictionary.setSurface(surfaceRepr);

    return is;
  }

  std::ostream&
  DictionarySerializer::
  writeSurface(ostream& os, const Dictionary& dictionary) const
  {
    auto dict = dictionary.getSurface();

    for (const auto& kv: dict) {
      for (const auto &word: kv.second) {
        os << word << endl;
      }
    }

    return os;
  }

  std::ostream&
  DictionarySerializer::
  writePath(ostream& os, const vector<string>& path) const
  {
    for (const auto& p: path) {
        os << p << endl;
    }

    return os;
  }

}