#pragma once

#include <vector>

namespace explorer {

  template<typename PointType, typename MeasureType>
  class Robot {
    public:
      virtual std::vector<PointType> findPath(PointType, PointType) = 0;

    private:
      virtual std::vector<PointType> search(PointType, PointType) = 0;
  };

}