#pragma once

#include "../surface/Surface.h"

#include <iostream>
#include <string>

using namespace std;

namespace explorer {

  template <typename PointType, typename MeasureType>
  class SurfaceSerializer {
    public:
      virtual std::istream& readSurface(istream&, Surface<PointType, MeasureType>&, bool lookForArgs = true) = 0;
      virtual std::ostream& writeSurface(ostream&, const Surface<PointType, MeasureType>&) = 0;
      virtual std::ostream& writePath(ostream&, const vector<PointType>&) = 0;
  };

}