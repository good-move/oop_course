#pragma once

#include <map>
#include <memory>
#include "surface/Planar.h"

namespace explorer {

  class AbstractPlanarCreator {
    public:
      virtual std::shared_ptr<Planar> create() const = 0;
  };

  template <typename T>
  class PlanarCreator : public AbstractPlanarCreator {
    public:
      virtual std::shared_ptr<Planar> create() const final override {
        return std::shared_ptr<Planar>(new T());
      }
  };

  class PlanarFactory {
    public:
      using id_type = std::string;
      using factory_map = std::map<id_type, std::shared_ptr<AbstractPlanarCreator>>;

      template<typename T>
      PlanarFactory& add(const id_type& id) {
        if (factory_.count(id) == 0) {
          factory_[id] = std::shared_ptr<PlanarCreator<T>>(new PlanarCreator<T>());
        }
        return *this;
      }

      std::shared_ptr<Planar> create(const id_type& id) {
        return factory_.at(id)->create();
      }

    private:
      factory_map factory_;
  };

}