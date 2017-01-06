#pragma once

#include "Planar.h"

using namespace std;

namespace explorer {

  class Cylinder : public Planar {
    public:
      size_t distance(const Point&, const Point&) const override final;
      vector<Point> lookup(const Point&) const override final;
  };

}