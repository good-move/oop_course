#include "../include/Parser.h"
#include "../include/serializer/PlanarSerializer.h"
#include "../include/surface/Torus.h"
#include "../include/surface/Plane.h"
#include "../include/surface/Cylinder.h"
#include "../include/robot/AStarRobot.h"
#include "../include/surface/Dictionary.h"
#include "../include/serializer/DictionarySerializer.h"
#include "../include/PlanarFactory.h"

#include <gtest/gtest.h>

using namespace std;
using namespace explorer;

#define MESSAGE(str) std::cout<<(str)<<std::endl;

const PlanarSerializer simpleSurfaceSerializer('_', '@', '.');
const DictionarySerializer dictionarySerializererializer{};


TEST(SurfaceTest, NegativeCasesOnInput)
{
  Plane plane;
  Dictionary dict;

  ifstream input;

  MESSAGE("Test for unknown char");
  input.open("maps/unknown_char.txt");
  EXPECT_THROW(simpleSurfaceSerializer.readSurface(input, plane), invalid_argument);
  input.close();

  MESSAGE("Test for bad surface dimensions");
  input.open("maps/bad_dimensions.txt");
  EXPECT_THROW(simpleSurfaceSerializer.readSurface(input, plane), invalid_argument);
  input.close();

  MESSAGE("Test for multiple start point");
  input.open("maps/multiple_start.txt");
  EXPECT_THROW(simpleSurfaceSerializer.readSurface(input, plane), logic_error);
  input.close();

  MESSAGE("Test for multiple finish point");
  input.open("maps/multiple_finish.txt");
  EXPECT_THROW(simpleSurfaceSerializer.readSurface(input, plane), logic_error);
  input.close();

  MESSAGE("Test for no starting point");
  input.open("maps/no_start_dict.txt");
  EXPECT_THROW(dictionarySerializererializer.readSurface(input, dict), invalid_argument);
  input.close();

  MESSAGE("Test for no finishing point");
  input.open("maps/no_finish_dict.txt");
  EXPECT_THROW(dictionarySerializererializer.readSurface(input, dict), invalid_argument);
  input.close();

//  add no_start and no_finish for simple surface

}

TEST(SimpleSurfaceTest, NegativeCasesInvalidPath)
{
  Plane plane;
  AStarRobot<Point, size_t, PointHash> robot(plane);

  ifstream input;

  input.open("maps/map1.txt");
  simpleSurfaceSerializer.readSurface(input, plane, false);
  input.close();

  MESSAGE("Starting from obstacle");
  EXPECT_THROW(robot.findPath(Point(0,0), Point(1,1)), std::invalid_argument);

  MESSAGE("Finishing at obstacle");
  EXPECT_THROW(robot.findPath(Point(1,1), Point(0,0)), std::invalid_argument);
}

TEST(SimpleSurfaceTest, NegativeCasesNoPath1)
{
  MESSAGE("No path for any simple surface");

  Plane plane;
  Cylinder cylinder;
  Torus torus;

  AStarRobot<Point, size_t, PointHash> robotP(plane);
  AStarRobot<Point, size_t, PointHash> robotC(cylinder);
  AStarRobot<Point, size_t, PointHash> robotT(torus);

  ifstream input;

  input.open("maps/no_path1.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);
  input.close();
  input.open("maps/no_path1.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();
  input.open("maps/no_path1.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  EXPECT_THROW(robotP.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
  EXPECT_THROW(robotC.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
  EXPECT_THROW(robotT.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);

}

TEST(SimpleSurfaceTest, NegativeCasesNoPath2)
{
  Plane plane;
  Cylinder cylinder;
  Torus torus;

  AStarRobot<Point, size_t, PointHash> robotP(plane);
  AStarRobot<Point, size_t, PointHash> robotC(cylinder);
  AStarRobot<Point, size_t, PointHash> robotT(torus);

  ifstream input;

  input.open("maps/no_path2.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);
  input.close();
  input.open("maps/no_path2.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();
  input.open("maps/no_path2.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  EXPECT_THROW(robotP.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
  EXPECT_THROW(robotC.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
  EXPECT_THROW(robotT.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);

}

TEST(SimpleSurfaceTest, NegativeCasesNoPath3)
{
  Plane plane;
  Cylinder cylinder;
  Torus torus;

  AStarRobot<Point, size_t, PointHash> robotP(plane);
  AStarRobot<Point, size_t, PointHash> robotC(cylinder);
  AStarRobot<Point, size_t, PointHash> robotT(torus);

  ifstream input;

  input.open("maps/no_path3.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);
  input.close();
  input.open("maps/no_path3.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();
  input.open("maps/no_path3.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  EXPECT_THROW(robotP.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
  EXPECT_THROW(robotC.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
  EXPECT_THROW(robotT.findPath(surfaceInfo.start, surfaceInfo.finish),
               std::invalid_argument);
}

TEST(SimpleSurfaceTest, TestTopology)
{
  Plane plane;
  Cylinder cylinder;
  Torus torus;

  ifstream input;

  input.open("maps/topology_difference.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);

  input.clear();
  input.seekg(0);
  simpleSurfaceSerializer.readSurface(input, cylinder);

  input.clear();
  input.seekg(0);
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  AStarRobot<Point, size_t, PointHash> robotP(plane);
  AStarRobot<Point, size_t, PointHash> robotC(cylinder);
  AStarRobot<Point, size_t, PointHash> robotT(torus);

  ASSERT_THROW(robotP.findPath(surfaceInfo.start, surfaceInfo.finish), std::invalid_argument);

  auto path = robotC.findPath(surfaceInfo.start, surfaceInfo.finish);
  ASSERT_EQ(path.size(), 11);

  path = robotT.findPath(surfaceInfo.start, surfaceInfo.finish);
  ASSERT_EQ(path.size(), 3);
}

TEST(SimpleSurfaceTest, TestPlane1)
{
  ifstream input;

  Plane plane;
  AStarRobot<Point, size_t, PointHash> robot(plane);

  input.open("maps/plane_has_path1.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);
  input.close();

  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 7);
}

TEST(SimpleSurfaceTest, TestPlane2)
{
  ifstream input;

  Plane plane;

  input.open("maps/plane_has_path2.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(plane);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 33);

}

TEST(SimpleSurfaceTest, TestPlane3)
{
  ifstream input;

  Plane plane;
  input.open("maps/plane_has_path3.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, plane);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(plane);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 12);
}

TEST(SimpleSurfaceTest, TestCylinder1)
{
  ifstream input;

  Cylinder cylinder;
  input.open("maps/cylinder_has_path1.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(cylinder);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 6);
}

TEST(SimpleSurfaceTest, TestCylinder2)
{
  ifstream input;

  Cylinder cylinder;

  input.open("maps/cylinder_has_path2.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(cylinder);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 18);
}

TEST(SimpleSurfaceTest, TestCylinder3)
{
  ifstream input;

  Cylinder cylinder;

  input.open("maps/cylinder_has_path3.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(cylinder);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 4);
}

TEST(SimpleSurfaceTest, TestTorus1)
{
  ifstream input;

  Torus torus;

  input.open("maps/torus_has_path1.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(torus);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 3);
}

TEST(SimpleSurfaceTest, TestTorus2)
{
  ifstream input;

  Torus torus;

  input.open("maps/torus_has_path2.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(torus);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 9);
}

TEST(SimpleSurfaceTest, TestTorus3)
{
  ifstream input;

  Torus torus;

  input.open("maps/torus_has_path3.txt");
  auto surfaceInfo = simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(torus);
  auto path = robot.findPath(surfaceInfo.start, surfaceInfo.finish);
  EXPECT_EQ(path.size(), 13);
}

TEST(Dictionary, TestDict1)
{
  ifstream input;

  Dictionary dict;
  input.open("dicts/custom_dict1.txt");
  dictionarySerializererializer.readSurface(input, dict, false);
  input.close();

  AStarRobot<string, size_t, std::hash<string>> robot(dict);

  auto path = robot.findPath("true", "truck");
  EXPECT_EQ(path.size(), 3);

  path = robot.findPath("mummy", "truck");
  EXPECT_EQ(path.size(), 10);

  ASSERT_THROW(robot.findPath("jack", "jones"), std::logic_error);

  path = robot.findPath("dump", "rum");
  EXPECT_EQ(path.size(), 3);

  ASSERT_THROW(robot.findPath("sick", "sick"), std::logic_error);
}

TEST(Dictionary, TestDict2)
{
  ifstream input;

  Dictionary dict;
  input.open("dicts/custom_dict2.txt");
  dictionarySerializererializer.readSurface(input, dict, false);
  input.close();

  AStarRobot<string, size_t, std::hash<string>> robot(dict);
  auto path = robot.findPath("cake", "chase");
  EXPECT_EQ(path.size(), 3);
}

TEST(Dictionary, TestDict3)
{
  ifstream input;

  Dictionary dict;
  input.open("dicts/custom_dict3.txt");
  dictionarySerializererializer.readSurface(input, dict, false);
  input.close();

  AStarRobot<string, size_t, std::hash<string>> robot(dict);
  auto path = robot.findPath("dog", "bowl");
  EXPECT_EQ(path.size(), 5);
}


int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
