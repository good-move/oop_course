#pragma once

#include "SimpleSurface.h"

using namespace std;

namespace explorer {

  class Cylinder : public SimpleSurface {
    public:
      size_t distance(Point, Point) const override;
      vector<Point> lookup(Point) const override;
  };

}