#pragma once

#include "Planar.h"

namespace explorer {

  class Torus : public Planar {
    public:
      size_t distance(const Point&, const Point&) const override final;
      vector<Point> lookup(const Point&) const override final;
  };

}