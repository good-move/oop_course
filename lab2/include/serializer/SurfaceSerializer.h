#pragma once

#include "../surface/Surface.h"

#include <iostream>
#include <string>

namespace explorer {

  template <typename SurfaceType>
  class SurfaceSerializer {
    public:
      using point_vector = typename SurfaceType::point_vector;
      using point_type = typename SurfaceType::point_type;

      struct surface_info_s {
          surface_info_s() : lookForEndpoints(true) {};
          point_type start, finish;
          bool lookForEndpoints;
      };

      using SurfaceInfo = struct surface_info_s;

      virtual SurfaceInfo readSurface(std::istream&, SurfaceType&, bool lookForEndpoints = true) const = 0;
      virtual void writeSurface(std::ostream&, const SurfaceType&) const = 0;
      virtual void writePath(std::ostream&, const SurfaceType&, const point_vector&) const = 0;
  };

}