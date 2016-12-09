#include <iostream>
#include <fstream>

#include "include/serializer/SimpleSurfaceSerializer.h"
#include "include/surface/Torus.h"
#include "include/surface/Plane.h"
#include "include/surface/Cylinder.h"
#include "include/robot/AStarRobot.h"

using namespace std;
using namespace explorer;

int main()
{
  Torus plane;

  ifstream file("map.txt");
  if (!file.is_open()) return 1;

  SimpleSurfaceSerializer serializer('_', '@', '.');

  Point start(0,0), finish(0,0);

  try {
    serializer.readSurface(file, plane);
  } catch (invalid_argument& e) {
    cout << e.what() << endl;
    return 1;
  }

  file.close();

  AStarRobot<Point, size_t, explorer::PointHash> robot(plane);
  auto path = robot.findPath(start, finish);

  ofstream output("path.txt");
  if (!output.is_open()) return 1;

  serializer.writePath(output, plane, path);

  output.close();

  return 0;
}