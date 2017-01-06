#include "../surface/Planar.h"
#include "./SurfaceSerializer.h"

namespace explorer {

  class PlanarSerializer : public SurfaceSerializer<Planar> {
    public:
      using SurfaceSerializer<Planar>::SurfaceInfo;
      using point_vector = typename SurfaceSerializer<Planar>::point_vector;
      using surface_points = typename Planar::surface_points;

      PlanarSerializer();
      PlanarSerializer(const char trailSign, const char obstacleSign, const char pathSign);

      virtual SurfaceInfo readSurface(std::istream&, Planar&, bool lookForEndpoints = true) const final override;
      virtual void writeSurface(std::ostream&, const Planar&) const final override;
      virtual void writePath(std::ostream&, const Planar&, const point_vector&) const final override;

    private:
      void addPoint(surface_points&, Point, unsigned char) const;
      bool isSurfaceValid(surface_points&) const;
      void writeToStream(std::ostream&, const surface_points&) const;

      const char trailSign_, obstacleSign_, pathSign_;
  };

}
