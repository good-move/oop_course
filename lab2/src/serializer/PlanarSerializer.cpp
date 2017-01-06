#include "../../include/serializer/PlanarSerializer.h"

using namespace std;

namespace explorer {

  PlanarSerializer::
  PlanarSerializer() :
  trailSign_('.'), obstacleSign_('X'), pathSign_('*')
  {}

  PlanarSerializer::
  PlanarSerializer(const char trailSign, const char obstacleSign, const char pathSign) :
    trailSign_(trailSign), obstacleSign_(obstacleSign), pathSign_(pathSign)
  {}

  SurfaceSerializer<Planar>::SurfaceInfo
  PlanarSerializer::
  readSurface(std::istream& is, Planar& surface, bool lookForEndpoints) const
  {
    if (surface.isBuilt()) {
      throw logic_error("Cannot initialize a surface twice.");
    }

    surface_points surfaceRepr;
    SurfaceInfo surfaceInfo;
    surfaceInfo.lookForEndpoints = lookForEndpoints;

    bool startFound = false;
    bool finishFound = false;
    string line;
    size_t x = 0, y = 0;

    while (getline(is, line)) {
      for (auto &c : line) {
        if (c == obstacleSign_) {
          this->addPoint(surfaceRepr, Point(x, y), Obstacle);
        }
        else {
          this->addPoint(surfaceRepr, Point(x, y), Trail);
          if (c == trailSign_){}
          else if (c == 'S' && lookForEndpoints) {
            if (startFound)
              throw logic_error("There must be only one start point");
            startFound = true;
            surfaceInfo.start = Point(x, y);
          } else if (c == 'F' && lookForEndpoints) {
            if (finishFound)
              throw logic_error("There must be only one finish point");
            finishFound = true;
            surfaceInfo.finish = Point(x, y);
          } else {
            throw invalid_argument(string{"Error. Unknown point type: "} + c);
          }
        }
        x += 1;
      }
      x = 0;
      y += 1;
    }

    if (this->isSurfaceValid(surfaceRepr)) {
      surface.setSurface(surfaceRepr);
    }

    return surfaceInfo;
  }

  void
  PlanarSerializer::
  writeSurface(std::ostream& os, const Planar& surface) const
  {
    this->writeToStream(os, surface.getSurface());
  };

  void
  PlanarSerializer::
  writePath(ostream& os, const Planar& surface, const vector<Point>& path) const
  {
    auto surfaceCopy = surface.getSurface();

    for (const auto& p : path) {
      surfaceCopy[p.y][p.x] = PathTile;
    }
    if (path.size() > 0) {
      Point start = path.front();
      Point finish = path.back();

      surfaceCopy[start.y][start.x] = 'S';
      surfaceCopy[finish.y][finish.x] = 'F';
    }

    this->writeToStream(os, surfaceCopy);
  }

  void
  PlanarSerializer::
  addPoint(surface_points& surface, Point point, unsigned char type) const
  {
    if (surface.size() <= point.y) {
      surface.push_back(std::vector<unsigned char>{});
    }

    surface[point.y].push_back(type);
  }

  bool
  PlanarSerializer::
  isSurfaceValid(surface_points& surface) const
  {
    if (surface.size() == 0 || surface[0].size() == 0) {
      return false;
    }

    size_t length = surface.size();
    size_t width = surface[0].size();

    for (size_t row = 0; row < length; ++row) {
      if (surface[row].size() != width) {
        throw invalid_argument(
                string("Error. Different row sizes are not allowed.\n") +
                "Row number " + to_string(row + 1) + " has size " + to_string(surface[row].size()) + ". "
                "Expected " + to_string(width) + "."
        );
      }
    }

    return true;
  }

  void
  PlanarSerializer::
  writeToStream(ostream& os, const surface_points& surface) const
  {
    for (const auto &row : surface) {
      for (const auto &cell : row) {
        switch (cell) {
          case Trail:
            os << trailSign_;
            break;
          case Obstacle:
            os << obstacleSign_;
            break;
          case PathTile:
            os << pathSign_;
            break;
          default:
            os << cell;
        }
      }
      os << endl;
    }
  }

}