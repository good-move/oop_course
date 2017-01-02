#pragma once

#include "../surface/Dictionary.h"
#include "SurfaceSerializer.h"

#include <string>

namespace explorer {

  class DictionarySerializer {
    public:
      std::istream& readSurface(istream&, Dictionary&, bool lookForArgs = true) const;
      std::ostream& writeSurface(ostream&, const Dictionary&) const;
      std::ostream& writePath(ostream&, const vector<string>&) const;
  };

}