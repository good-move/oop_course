#include "../../include/serializer/DictionarySerializer.h"

using namespace std;

namespace explorer {



  SurfaceSerializer<Dictionary>::SurfaceInfo
  DictionarySerializer::
  readSurface(istream& is, Dictionary& dictionary, bool lookForEndpoints) const
  {
    if (dictionary.isBuilt()) {
      throw logic_error("Cannot initialize a surface twice.");
    }

    unordered_map<size_t, vector<string>> surfaceRepr;

    // assuming the input dictionary has one word per line
    string word;
    SurfaceInfo surfaceInfo {};
    surfaceInfo.lookForEndpoints = lookForEndpoints;

    if (lookForEndpoints) {
      if (!getline(is, word)) { // start word
        throw invalid_argument("Start point must be provided");
      }
      surfaceInfo.start = word;

      if (!getline(is, word)) { // finish word
        throw invalid_argument("Finish point must be provided");
      }
      surfaceInfo.finish = word;
    }

    while (getline(is, word)) {
      if (word.length() > 0) {
        surfaceRepr[word.length()].push_back(word);
      }
    }

    dictionary.setSurface(surfaceRepr);

    return surfaceInfo;
  }

  void
  DictionarySerializer::
  writeSurface(ostream& os, const Dictionary& dictionary) const
  {
    auto dict = dictionary.getSurface();

    for (const auto& kv: dict) {
      for (const auto &word: kv.second) {
        os << word << endl;
      }
    }
  }

  void
  DictionarySerializer::
  writePath(ostream& os, const Dictionary&, const point_vector& path) const
  {
    for (const auto& p: path) {
        os << p << endl;
    }
  }

}