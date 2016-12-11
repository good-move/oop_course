#include "../surface/SimpleSurface.h"
#include "./SurfaceSerializer.h"

namespace explorer {

  class SimpleSurfaceSerializer {
    public:
      SimpleSurfaceSerializer();
      SimpleSurfaceSerializer(char trailSign, char obstacleSign, char pathSign);

      std::istream& readSurface(istream&, SimpleSurface&, bool lookForArgs = true) const;
      std::ostream& writeSurface(ostream&, const SimpleSurface&) const;
      std::ostream& writePath(ostream&, const SimpleSurface&, const vector<Point>&) const;
    private:
      void addPoint(vector<vector<unsigned char>>&, Point, unsigned char) const;
      bool isSurfaceValid(vector<vector<unsigned char>>&) const;
      void writeToStream(ostream&, const vector<vector<unsigned char>>&) const;

      const char trailSign_, obstacleSign_, pathSign_;
  };

}
