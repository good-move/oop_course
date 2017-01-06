#pragma once

#include "Surface.h"
#include <string>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::string;

namespace explorer {

  class Dictionary : public Surface<string, size_t> {
    public:
      using point_type = string;
      using measure_type = size_t;
      using point_vector = vector<string>;
      using surface_points = unordered_map<measure_type, point_vector>;

      Dictionary();
      bool checkPath(const point_vector&, const point_type&, const point_type&) const override;
      size_t distance(const point_type&, const point_type&) const override;
      point_vector lookup(const point_type&) const override;
      bool isWalkable(const point_type&) const override;

      surface_points getSurface() const;
      void setSurface(const surface_points&);
      bool isBuilt() const;

    private:
      bool isInDict(const point_type&) const;
      bool isWordValid(const point_type&) const;

      surface_points dictionary_;
      bool isBuilt_;
  };

}