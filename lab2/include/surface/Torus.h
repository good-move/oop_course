#pragma once

#include "SimpleSurface.h"

namespace explorer {

  class Torus : public SimpleSurface {
    public:
      size_t distance(const Point&, const Point&) const override final;
      vector<Point> lookup(const Point&) const override final;
  };

}