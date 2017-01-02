#include "../include/Parser.h"
#include "../include/serializer/SimpleSurfaceSerializer.h"
#include "../include/surface/Torus.h"
#include "../include/surface/Plane.h"
#include "../include/surface/Cylinder.h"
#include "../include/robot/AStarRobot.h"
#include "../include/surface/Dictionary.h"
#include "../include/serializer/DictionarySerializer.h"

#include <gtest/gtest.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace explorer;

#define MESSAGE(str) std::cout<<(str)<<std::endl;

const SimpleSurfaceSerializer simpleSurfaceSerializer('_', '@', '.');
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
  simpleSurfaceSerializer.readSurface(input, plane);
  input.close();
  input.open("maps/no_path1.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();
  input.open("maps/no_path1.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  EXPECT_THROW(robotP.findPath(plane.start, plane.finish),
               std::invalid_argument);
  EXPECT_THROW(robotC.findPath(plane.start, plane.finish),
               std::invalid_argument);
  EXPECT_THROW(robotT.findPath(plane.start, plane.finish),
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
  simpleSurfaceSerializer.readSurface(input, plane);
  input.close();
  input.open("maps/no_path2.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();
  input.open("maps/no_path2.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  EXPECT_THROW(robotP.findPath(plane.start, plane.finish),
               std::invalid_argument);
  EXPECT_THROW(robotC.findPath(plane.start, plane.finish),
               std::invalid_argument);
  EXPECT_THROW(robotT.findPath(plane.start, plane.finish),
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
  simpleSurfaceSerializer.readSurface(input, plane);
  input.close();
  input.open("maps/no_path3.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();
  input.open("maps/no_path3.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  EXPECT_THROW(robotP.findPath(plane.start, plane.finish),
               std::invalid_argument);
  EXPECT_THROW(robotC.findPath(plane.start, plane.finish),
               std::invalid_argument);
  EXPECT_THROW(robotT.findPath(plane.start, plane.finish),
               std::invalid_argument);
}

TEST(SimpleSurfaceTest, TestTopology)
{
  Plane plane;
  Cylinder cylinder;
  Torus torus;

  ifstream input;

  input.open("maps/topology_difference.txt");
  simpleSurfaceSerializer.readSurface(input, plane);

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

  ASSERT_THROW(robotP.findPath(plane.start, plane.finish), std::invalid_argument);

  auto path = robotC.findPath(cylinder.start, cylinder.finish);
  ASSERT_EQ(path.size(), 11);

  path = robotT.findPath(torus.start, torus.finish);
  ASSERT_EQ(path.size(), 3);
}

TEST(SimpleSurfaceTest, TestPlane)
{
  ifstream input;

  Plane plane;
  AStarRobot<Point, size_t, PointHash> robot(plane);

  input.open("maps/plane_has_path1.txt");
  simpleSurfaceSerializer.readSurface(input, plane);
  input.close();

  auto path = robot.findPath(plane.start, plane.finish);
  EXPECT_EQ(path.size(), 7);

  Plane plane1;

  input.open("maps/plane_has_path2.txt");
  simpleSurfaceSerializer.readSurface(input, plane1);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot1(plane1);
  path = robot1.findPath(plane1.start, plane1.finish);
  EXPECT_EQ(path.size(), 33);


  Plane plane2;
  input.open("maps/plane_has_path3.txt");
  simpleSurfaceSerializer.readSurface(input, plane2);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot2(plane2);
  path = robot2.findPath(plane2.start, plane2.finish);
  EXPECT_EQ(path.size(), 12);
}

TEST(SimpleSurfaceTest, TestCylinder)
{
  ifstream input;

  Cylinder cylinder;
  input.open("maps/cylinder_has_path1.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(cylinder);
  auto path = robot.findPath(cylinder.start, cylinder.finish);
  EXPECT_EQ(path.size(), 6);


  Cylinder cylinder1;
  input.open("maps/cylinder_has_path2.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder1);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot1(cylinder1);
  path = robot1.findPath(cylinder1.start, cylinder1.finish);
  EXPECT_EQ(path.size(), 18);

  Cylinder cylinder2;

  input.open("maps/cylinder_has_path3.txt");
  simpleSurfaceSerializer.readSurface(input, cylinder2);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot2(cylinder2);
  path = robot2.findPath(cylinder2.start, cylinder2.finish);
  EXPECT_EQ(path.size(), 4);
}

TEST(SimpleSurfaceTest, TestTorus)
{
  ifstream input;

  Torus torus;
  input.open("maps/torus_has_path1.txt");
  simpleSurfaceSerializer.readSurface(input, torus);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot(torus);
  auto path = robot.findPath(torus.start, torus.finish);
  EXPECT_EQ(path.size(), 3);


  Torus torus1;
  input.open("maps/torus_has_path2.txt");
  simpleSurfaceSerializer.readSurface(input, torus1);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot1(torus1);
  path = robot1.findPath(torus1.start, torus1.finish);
  EXPECT_EQ(path.size(), 9);

  Torus torus2;

  input.open("maps/torus_has_path3.txt");
  simpleSurfaceSerializer.readSurface(input, torus2);
  input.close();

  AStarRobot<Point, size_t, PointHash> robot2(torus2);
  path = robot2.findPath(torus2.start, torus2.finish);
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
