#pragma once

#include <vector>

namespace explorer {

  template<typename PointType, typename MeasureType>
  class Robot {
    public:
      using point_type = PointType;
      using measure_type = MeasureType;
      using point_vector = std::vector<PointType>;
      using surface_type = Surface<PointType, MeasureType>;

      Robot(const surface_type& surface) : surface_(surface) {};
      point_vector findPath(point_type start, point_type finish) {
        point_vector path = this->search(start, finish);
        surface_.checkPath(path, start, finish); // throws exception in case of a bad path
        return path;
      }

    protected:
      const surface_type& surface_;

    private:
      virtual point_vector search(const point_type&, const point_type&) = 0;
  };

}