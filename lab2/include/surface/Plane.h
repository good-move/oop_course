#pragma once

#include "SimpleSurface.h"

namespace explorer {

  class Plane : public SimpleSurface {
    public:
      size_t distance(const Point&, const Point&) const override final;
      vector<Point> lookup(const Point&) const override final;
  };

}