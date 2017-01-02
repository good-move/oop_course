#include "../../include/surface/SimpleSurface.h"

using namespace std;

namespace explorer {

  ostream& operator<<(ostream& os, const Point& point)
  {
    os << point.toString();

    return os;
  }

/* ************************** public methods SimpleSurface ************************** */

  SimpleSurface::
  SimpleSurface()
  {
    isBuilt_ = false;
  }

  bool
  SimpleSurface::
  checkPath(const pointVector& path, const Point& start, const Point& finish) const
  {
    if (!isPointValid(start) || !isPointValid(finish)) {
      throw invalid_argument("Start or finish point is invalid.");
    }
    if (path.size() == 0) {
      throw invalid_argument("No path provided");
    }
    if (path.front() != start || path.back() != finish) {
      throw logic_error("Error: start(finish) argument " \
                        "doesn't match path start(finish)");
    }

    Point current = start;
    for (auto &point : path) {
      if (!isPointValid(point)) {
        throw logic_error("The point " + point.toString() +
                          " is outside the surface bounds or is an obstacle.");
      }
      if (this->distance(current, point) > 1) {
        throw logic_error("Error: can't jump over cells");
      }
      current = point;
    }

    return true;
  }

  bool
  SimpleSurface::
  isWalkable(const Point& point) const
  {
    return this->isPointValid(point);
  }

  bool
  SimpleSurface::
  setSurface(const surface_points& surface)
  {
    if (isBuilt_) {
      return false;
    }

    surface_ = surface;
    isBuilt_ = true;

    return true;
  }

  surface_points
  SimpleSurface::
  getSurface() const
  {
    return surface_;
  }

/* ********************** private methods SimpleSurface ********************** */

  bool
  SimpleSurface::
  isPointValid(const Point& point) const
  {
    return this->isPointWithinBounds(point) &&
           !this->isObstacle(surface_[point.y][point.x]);
  }

  bool
  SimpleSurface::
  isPointWithinBounds(const Point& point) const
  {
    return point.x < this->getWidth() && point.y < this->getHeight();
  }

  bool
  SimpleSurface::
  isObstacle(bool pointType) const
  {
    return pointType == Obstacle;
  }

  size_t
  SimpleSurface::
  getWidth() const
  {
    return surface_.size() > 0 ? surface_[0].size() : 0;
  }

  size_t
  SimpleSurface::
  getHeight() const
  {
    return surface_.size();
  }

  bool
  SimpleSurface::
  isBuilt() const
  {
    return isBuilt_;
  }

/* ************************** struct point_s ************************** */

  point_s::
  point_s() : x(0), y(0)
  {}

  point_s::
  point_s(size_t x, size_t y) : x(x), y(y)
  {}

  bool
  point_s::
  operator==(point_s point) const
  {
    return this->x == point.x && this->y == point.y;
  }

  bool
  point_s::
  operator!=(point_s point) const
  {
    return !this->operator==(point);
  }

  string
  point_s::toString() const
  {
    return "(" + to_string(this->x) + ", " + to_string(this->y) + ")";
  }

/* ************************** class PointHash ************************** */

  size_t
  PointHash::
  operator()(const Point& point) const
  {
    return (31 + point.x) * 31 + point.y;
  }

}
