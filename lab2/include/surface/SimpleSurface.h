#pragma once

#include "Surface.h"

#include <unordered_map>
#include <string>
#include <cmath>

using namespace std;

namespace explorer {

  enum PointTypes : unsigned char {
      Trail = 0, Obstacle, PathTile
  };

  using Point = struct point_s;

  struct point_s {
    public:
      point_s();
      point_s(size_t, size_t);
      bool operator==(point_s point) const;
      bool operator!=(point_s) const;
      string toString() const;

      size_t x, y;
  };

  class PointHash {
    public:
      size_t operator()(const Point& arg) const;
  };

  class SimpleSurface : public Surface<Point, size_t> {
    public:
      SimpleSurface();
      virtual size_t distance(Point, Point) const = 0;
      virtual vector<Point> lookup(Point) const = 0;
      virtual bool isWalkable(Point) const override;
      bool checkPath(vector<Point>, Point, Point) const override;
      size_t getWidth() const;
      size_t getHeight() const;
      bool isBuilt();
      bool setSurface(const vector<vector<unsigned char>>& surface);
      vector<vector<unsigned char>> getSurface() const;

    protected:
      vector<vector<unsigned char>> surface_;
      bool isBuilt_;

      bool isPointWithinBounds(Point) const;
      bool isPointValid(Point) const;
      bool isObstacle(bool) const;
  };

}