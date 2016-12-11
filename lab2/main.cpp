#include "include/Parser.h"
#include "include/serializer/SimpleSurfaceSerializer.h"
#include "include/surface/Torus.h"
#include "include/surface/Plane.h"
#include "include/surface/Cylinder.h"
#include "include/robot/AStarRobot.h"
#include "include/surface/Dictionary.h"
#include "include/serializer/DictionarySerializer.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace explorer;

template <typename PointType, typename MeasureType, typename Hash>
vector<PointType> runRobot(const Surface<PointType, MeasureType>&, size_t);

int main(int argc, char* argv[])
{
  try {
    Parser parser;
    parser.processCommandArgs(argc, argv);
    string topology = parser.getTopology();

    ifstream input (parser.getIFile());
    ofstream output (parser.getOFile());

    if (topology == "words") {
      DictionarySerializer serializer;
      Dictionary surface;
      serializer.readSurface(input, surface);
      auto path = runRobot<string, size_t, std::hash<string>>(surface, parser.getLengthLimit());
      serializer.writePath(output, path);
    } else {
      SimpleSurfaceSerializer serializer('_', '@', '.');
      if (topology == "plane") {
        Plane surface;
        serializer.readSurface(input, surface);
        auto path = runRobot<Point, size_t, PointHash>(surface, parser.getLengthLimit());
        serializer.writePath(output, surface, path);
      } else if (topology == "cylinder") {
        Cylinder surface;
        serializer.readSurface(input, surface);
        auto path = runRobot<Point, size_t, PointHash>(surface, parser.getLengthLimit());
        serializer.writePath(output, surface, path);
      } else if (topology == "torus") {
        Torus surface;
        serializer.readSurface(input, surface);
        auto path = runRobot<Point, size_t, PointHash>(surface, parser.getLengthLimit());
        serializer.writePath(output, surface, path);
      } else {
        throw invalid_argument("Cannot initialize surface of unknown topology");
      }
    }

    input.close();
    output.close();
  } catch (const invalid_argument& e) {
    cout << e.what() << endl;
    return 1;
  } catch (const logic_error& e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

template<typename PointType, typename MeasureType, typename Hash>
vector<PointType> runRobot(const Surface<PointType, MeasureType>& surface, size_t sizeLimit)
{
  AStarRobot<PointType, MeasureType, Hash> robot(surface);
  auto path = robot.findPath(surface.start, surface.finish);
  if (path.size() > sizeLimit) {
    path.clear();
    cout << "Path exceeds limit length of " << sizeLimit <<": leaving it out." << endl;
  }
  return path;
}
