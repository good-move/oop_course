#pragma once

#include "Surface.h"
#include <string>
#include <numeric>

using namespace std;

namespace explorer {
  
  class Dictionary : public Surface<string, size_t> {
    public:
      Dictionary();
      bool checkPath(vector<string>, string, string) const override;
      size_t distance(const string&, const string&) const override;
      vector<string> lookup(string) const override;
      bool isWalkable(string) const override;

      unordered_map<size_t, vector<string>> getSurface() const;
      void setSurface(const unordered_map<size_t, vector<string>>&);
      bool isBuilt() const;


    private:
      bool isInDict(string) const;
      bool isWordValid(string) const;

      unordered_map<size_t, vector<string>> dictionary_;
      bool isBuilt_;
  };

}