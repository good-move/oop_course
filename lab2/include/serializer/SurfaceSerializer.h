#pragma once

#include "../surface/Surface.h"

namespace explorer {

  template<typename PointType, typename MeasureType>
  class SurfaceSerializer {
    public:
      virtual void readSurface(std::istream&, Surface<PointType, MeasureType>) = 0;
      virtual void writeSurface(std::ostream&, Surface<PointType, MeasureType>) = 0;
  };

} // end explorer namespace
