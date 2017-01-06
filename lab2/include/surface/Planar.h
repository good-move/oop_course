#pragma once

#include "Surface.h"

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::vector;
using std::string;
using std::max;
using std::min;


namespace explorer {

  using Point = struct point_s;

  enum PointTypes : unsigned char {
      Trail = 0, Obstacle, PathTile
  };


  struct point_s {
    public:
      point_s();
      point_s(size_t, size_t);
      bool operator==(point_s) const;
      bool operator!=(point_s) const;
      string toString() const;

      size_t x, y;
  };

  class PointHash {
    public:
      size_t operator()(const Point&) const;
  };

  class Planar : public Surface<Point, size_t> {
    public:
      using surface_points = vector<vector<unsigned char>>;
      using point_vector = vector<Point>;

      Planar();
      virtual bool isWalkable(const Point&) const override;

      bool checkPath(const point_vector&, const Point&, const Point&) const override;
      size_t getWidth() const;
      size_t getHeight() const;
      bool isBuilt() const;
      bool setSurface(const surface_points& surface);
      surface_points getSurface() const;

    protected:
      bool isPointWithinBounds(const Point&) const;
      bool isPointValid(const Point&) const;
      bool isObstacle(bool) const;

      surface_points surface_;
      bool isBuilt_;
  };

}