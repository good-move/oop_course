#include "../../include/surface/SimpleSurface.h"

using namespace std;

namespace explorer {

/* ************************** public methods SimpleSurface ************************** */

  SimpleSurface::
  SimpleSurface()
  {
    isBuilt_ = false;
  }

  bool
  SimpleSurface::
  checkPath(vector<Point> path, Point start, Point finish) const
  {
    if (!isPointValid(start) || !isPointValid(finish)) {
      cout << "Start or finish point is invalid" << endl;
      return false;
    }
    if (path.size() == 0) {
      cout << "No path provided" << endl;
      return false;
    }
    if (path[0] != start || path[path.size() - 1] != finish) {
      cout << "Error: start(finish) argument doesn't match path start(finish)" << endl;
      return false;
    }

    Point current = start;
    for (auto &point : path) {
      if (!isPointValid(point)) {
        cout << "The point " << point.toString() << " is outside the surface bounds or is an obstacle." << endl;
        return false;
      }
      if (this->distance(current, point) > 1) {
        cout << "Error: can't jump over cells" << endl;
        return false;
      }
      current = point;
    }

    return true;
  }

  bool
  SimpleSurface::
  isWalkable(Point point) const
  {
    return this->isPointValid(point);
  }

/* ********************** private methods SimpleSurface ********************** */

  bool
  SimpleSurface::
  isPointValid(Point point) const
  {
    return this->isPointWithinBounds(point) && !this->isObstacle(surface_[point.y][point.x]);
  }

  bool
  SimpleSurface::
  isPointWithinBounds(Point point) const
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
  isBuilt()
  {
    return isBuilt_;
  }

  bool
  SimpleSurface::
  setSurface(const vector<vector<unsigned char>>& surface)
  {
    if (isBuilt_) {
      return false;
    }

    surface_ = surface;
    isBuilt_ = true;

    return true;
  }

  vector<vector<unsigned char>>
  SimpleSurface::
  getSurface() const
  {
    return surface_;
  }

/* ************************** struct point_s ************************** */

  point_s::
  point_s() :
          x(0), y(0)
  {}

  point_s::
  point_s(size_t x, size_t y) :
          x(x), y(y)
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
  point_s::
  toString() const
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


