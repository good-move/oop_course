#pragma once

#include <vector>

namespace explorer {

  template<typename PointType, typename MeasureType>
  class Surface {
    public:
      virtual bool checkPath(const std::vector<PointType>&, const PointType&, const PointType&) const = 0;
      virtual MeasureType distance(const PointType&, const PointType&) const = 0;
      virtual std::vector<PointType> lookup(const PointType&) const = 0;
      virtual bool isWalkable(const PointType&) const = 0;

      PointType start, finish;
  };

}