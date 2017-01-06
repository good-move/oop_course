#pragma once

#include "../3rd_party/anyoption/anyoption.h"
#include "surface/Surface.h"

#include <string>

namespace explorer {

  class Parser {
    public:
      Parser();
      ~Parser();

      bool processCommandArgs(int, char*[]);
      string getTopology() const;
      size_t getLengthLimit() const;
      string getIFile() const;
      string getOFile() const;

    private:
      void setDescription();

      AnyOption* opt_;
  };

}