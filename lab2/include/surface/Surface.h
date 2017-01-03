#pragma once

#include <vector>

namespace explorer {

  template<typename PointType, typename MeasureType>
  class Surface {
    public:
      using point_vector = std::vector<PointType>;
      using point_type = PointType;

      virtual bool checkPath(const point_vector&, const PointType&, const PointType&) const = 0;
      virtual MeasureType distance(const PointType&, const PointType&) const = 0;
      virtual point_vector lookup(const PointType&) const = 0;
      virtual bool isWalkable(const PointType&) const = 0;
  };

}