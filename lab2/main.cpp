#include "include/Parser.h"
#include "include/serializer/PlanarSerializer.h"
#include "include/surface/Torus.h"
#include "include/surface/Plane.h"
#include "include/surface/Cylinder.h"
#include "include/robot/AStarRobot.h"
#include "include/surface/Dictionary.h"
#include "include/serializer/DictionarySerializer.h"
#include "include/PlanarFactory.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace explorer;

template <typename SurfaceType, typename PointType, typename MeasureType, typename Hash>
void runRobot(
        SurfaceSerializer<SurfaceType>&,
        SurfaceType&,
        const Parser&);

int main(int argc, char* argv[])
{
  try {
    Parser parser;
    if (!parser.processCommandArgs(argc, argv)) {
      return 0;
    }

    const string& topology = parser.getTopology();

    if (topology == "words") {
      Dictionary dictionary;
      DictionarySerializer dictionarySerializer;
      runRobot<Dictionary, string, size_t, std::hash<string>>(
              dictionarySerializer,
              dictionary,
              parser);
    } else {
      PlanarFactory factory;
      factory.add<Plane>("plane").add<Cylinder>("cylinder").add<Torus>("torus");
      auto surface = factory.create(topology);

      PlanarSerializer serializer('_', '@', '.');
      runRobot<Planar, Point, size_t, PointHash>(
              serializer,
              *surface,
              parser);
    }

  } catch (const invalid_argument& e) {
    cout << e.what() << endl;
    return 1;
  } catch (const out_of_range& e) {
    cout << e.what() << endl;
    return 1;
  } catch (const logic_error& e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

template<typename SurfaceType, typename PointType, typename MeasureType, typename Hash>
void runRobot(
        SurfaceSerializer<SurfaceType>& serializer,
        SurfaceType& surface,
        const Parser& parser)
{

  ifstream input (parser.getIFile());
  ofstream output (parser.getOFile());

  auto surfaceInfo = serializer.readSurface(input, surface);

  AStarRobot<PointType, MeasureType, Hash> robot(surface);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  if (path.size() > parser.getLengthLimit()) {
    path.clear();
    cout << "Path exceeds limit length of " <<
            parser.getLengthLimit() <<": leaving it out." << endl;
  }

  serializer.writePath(output, surface, path);

  input.close();
  output.close();
}
