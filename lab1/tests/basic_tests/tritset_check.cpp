//
// Created by alexey on 23.10.16.
//


#include <gtest/gtest.h>
#include "../../src/TritSet.h"

using namespace alexgm;

TEST(TritSet, ZeroLengthTritSet) {

  TritSet set(0);

  EXPECT_EQ(set.length(), 0);
  EXPECT_EQ(set.capacity(), 0);


  for (int i = -10; i < 20; ++i) {
    ASSERT_EQ(Unknown, (Trit) set[0]);
  }

  set.shrink();
  EXPECT_EQ(set.length(), 0);
  EXPECT_EQ(set.capacity(), 0);

  set.trim(15);
  EXPECT_EQ(set.length(), 0);
  EXPECT_EQ(set.capacity(), 0);

  set.trim(0);
  EXPECT_EQ(set.length(), 0);
  EXPECT_EQ(set.capacity(), 0);


  set.trim(-5);
  EXPECT_EQ(set.length(), 0);
  EXPECT_EQ(set.capacity(), 0);
}

TEST(TritSet, ZeroLengthTritSetOperations) {
  TritSet emptySet(0);
  TritSet set(10);

  set[0] = set[3] = set[8] = True;
  set[2] = set[6] = False;

  TritSet result = set & emptySet;
  for (size_t i = 0; i < set.length(); ++i) {
    EXPECT_EQ((Trit)result[i], (Trit)set[i] & Unknown);
  }
}

TEST(TritSet, OperatorSquareBrakets) {

}

TEST(TritSet, CopyAssignmentOperator) {
  TritSet setA(10);
  TritSet setB(10);
  TritSet setC(20);
}
