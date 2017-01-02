#include "../../include/serializer/SimpleSurfaceSerializer.h"

namespace explorer {

  SimpleSurfaceSerializer::
  SimpleSurfaceSerializer() :
  trailSign_('.'), obstacleSign_('X'), pathSign_('*')
  {}

  SimpleSurfaceSerializer::
  SimpleSurfaceSerializer(char trailSign, char obstacleSign, char pathSign) :
    trailSign_(trailSign), obstacleSign_(obstacleSign), pathSign_(pathSign)
  {}

  std::istream&
  SimpleSurfaceSerializer::
  readSurface(std::istream& is, SimpleSurface& surface, bool lookForArgs) const
  {
    if (surface.isBuilt()) {
      throw logic_error("Cannot initialize a surface twice.");
    }

    vector<vector<unsigned char>> surfaceRepr;

    bool startFound = false;
    bool finishFound = false;
    string line;
    size_t x = 0, y = 0;

    while (getline(is, line)) {
      for (auto &c : line) {
        if (c == obstacleSign_) {
          this->addPoint(surfaceRepr, Point(x, y), Obstacle);
        }
        else if (c == trailSign_) {
            this->addPoint(surfaceRepr, Point(x, y), Trail);
        }
        else if (c == 'S' && lookForArgs) {
          if (startFound) {
            throw logic_error("There must be only one start point");
          }
          startFound = true;
          surface.start = Point(x,y);
          this->addPoint(surfaceRepr, Point(x, y), Trail);
        }
        else if (c == 'F' && lookForArgs) {
          if (finishFound) {
            throw logic_error("There must be only one finish point");
          }
          finishFound = true;
          surface.finish = Point(x,y);
          this->addPoint(surfaceRepr, Point(x, y), Trail);
        }
        else {
          throw std::invalid_argument(string{"Error. Unknown point type: "} + c);
        }
        x += 1;
      }
      x = 0;
      y += 1;
    }

    if (this->isSurfaceValid(surfaceRepr)) {
      surface.setSurface(surfaceRepr);
    }

    return is;
  }

  std::ostream&
  SimpleSurfaceSerializer::
  writeSurface(std::ostream& os, const SimpleSurface& surface) const
  {
    this->writeToStream(os, surface.getSurface());

    return os;
  };

  std::ostream&
  SimpleSurfaceSerializer::
  writePath(ostream& os, const SimpleSurface& surface, const vector<Point>& path) const
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

    return os;
  }

  void
  SimpleSurfaceSerializer::
  addPoint(vector<vector<unsigned char>>& surface, Point point, unsigned char type) const
  {
    if (surface.size() <= point.y) {
      surface.push_back(std::vector<unsigned char>{});
    }

    surface[point.y].push_back(type);
  }

  bool
  SimpleSurfaceSerializer::
  isSurfaceValid(vector<vector<unsigned char>>& surface) const
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
  SimpleSurfaceSerializer::
  writeToStream(ostream& os, const vector<vector<unsigned char>>& surface) const
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