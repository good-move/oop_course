#include "../../include/surface/Cylinder.h"

namespace explorer {

  size_t
  Cylinder::
  distance(const Point& p1, const Point& p2) const
  {
    size_t distY = max(p1.y, p2.y) - min(p1.y, p2.y);
    size_t distX = max(p1.x, p2.x) - min(p1.x, p2.x);

    distX = min(this->getWidth() - distX, distX);

    return distX + distY;
  }

  vector<Point>
  Cylinder::
  lookup(const Point& point) const
  {
    vector<Point> neighborhood;

    if (!this->isPointWithinBounds(point)) {
      return neighborhood;
    }

    if (point.x < this->getWidth() - 1)
      neighborhood.push_back(Point(point.x + 1, point.y));
    else
      neighborhood.push_back(Point(0, point.y));

    if (point.x > 0)
      neighborhood.push_back(Point(point.x - 1, point.y));
    else
      neighborhood.push_back(Point(this->getWidth() - 1, point.y));

    if (point.y < this->getHeight() - 1)
      neighborhood.push_back(Point(point.x, point.y+1));

    if (point.y > 0)
      neighborhood.push_back(Point(point.x, point.y-1));

    return neighborhood;
  }

}