#pragma once

#include "SimpleSurface.h"

namespace explorer {

  class Plane : public SimpleSurface {
    public:
      size_t distance(Point, Point) const override;
      vector<Point> lookup(Point) const override;
  };

}