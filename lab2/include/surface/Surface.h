#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

namespace explorer {

  template<typename PointType, typename MeasureType>
  class Surface {
    public:
      virtual bool checkPath(std::vector<PointType>, PointType, PointType) const = 0;
      virtual MeasureType distance(PointType, PointType) const = 0;
      virtual std::vector<PointType> lookup(PointType) const = 0;
      virtual bool isWalkable(PointType) const = 0;

  };

}