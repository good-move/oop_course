#pragma once

#include "../surface/Dictionary.h"
#include "SurfaceSerializer.h"

#include <string>

namespace explorer {

  class DictionarySerializer : public SurfaceSerializer<Dictionary>{
    public:
      using SurfaceSerializer<Dictionary>::SurfaceInfo;
      using typename SurfaceSerializer<Dictionary>::point_vector;

      virtual SurfaceInfo readSurface(std::istream&, Dictionary&, bool lookForEndpoints = true) const final override;
      virtual void writePath(std::ostream&, const Dictionary&, const point_vector&) const final override ;
      virtual void writeSurface(std::ostream&, const Dictionary&) const final override;
  };

}