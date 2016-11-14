#include "../../src/TritSet.h"
#include "../lib/g-test/include/gtest/gtest.h"
#include "../lib/g-test/include/gtest/internal/gtest-internal.h"
#include "../lib/g-test/include/gtest/internal/gtest-port.h"
//#include <gtest/gtest.h>

using namespace std;
using namespace alexgm;

#define START_MSG(msg) cout<<endl; cout <<"*** "<< (msg)<<" ***"<< endl;
#define TEST_MSG(msg) cout << (msg) << endl;
#define SUCCESS_MSG cout << "SUCCESS" << endl; cout<<endl;


TEST(TritSet, TestsFromAssignment) {

  START_MSG("TestsFromAssignment");

  TritSet set(1000);
  
  size_t allocLength = set.capacity();
  ASSERT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(uint)) << "Wrong size";

  set[1000000000] = Unknown;
  ASSERT_EQ(allocLength, set.capacity()) << "Allocated extra memory where it shouldn't have";

  if(set[2000000000] == True){}
  ASSERT_TRUE(allocLength == set.capacity()) << "No memory operations asserted";

  set[1000000000] = True;
  ASSERT_TRUE(allocLength < set.capacity()) << "Expected to allocate more memory";


  allocLength = set.capacity();
  set[1000000000] = Unknown;
  set[1000000] = False;
  ASSERT_EQ(allocLength, set.capacity()) << "No memory operations asserted";


  set.shrink();
  ASSERT_TRUE(allocLength > set.capacity()) << "shrink() did not free enough space";

  TritSet setA(3);
  TritSet setB(500);
  setA[0] = setA[2] = True;
  setB[1] = False;
  setB[2] = True;

  TritSet setC = (setB & setA) | setB;
  ASSERT_EQ(setC.capacity(), setB.capacity()) << "Expected to allocate memory to match r-value size";

  SUCCESS_MSG;
}

TEST(TritSet, ZeroLengthTritSet) {

  START_MSG("ZeroLengthTritSet");

  TritSet emptySet(0);

  TEST_MSG("Assert length and capacity to equal 0");

  ASSERT_EQ(emptySet.length(), 0u);
  ASSERT_EQ(emptySet.capacity(), 0u);

  TEST_MSG("Assert every trit equals Unknown");

  for (int i = -10; i < 20; ++i) {
    ASSERT_EQ(Unknown, emptySet[i]);
  }

  TEST_MSG("Shrinking empty set");
  emptySet.shrink();
  EXPECT_EQ(emptySet.length(), 0u);
  EXPECT_EQ(emptySet.capacity(), 0u);

  TEST_MSG("Trim empty set to size 15");
  emptySet.trim(15);
  EXPECT_EQ(emptySet.length(), 0u);
  EXPECT_EQ(emptySet.capacity(), 0u);

  TEST_MSG("Trim empty set to size 0");
  emptySet.trim(0);
  EXPECT_EQ(emptySet.length(), 0u);
  EXPECT_EQ(emptySet.capacity(), 0u);

  TritSet set(10u);

  set[0] = set[3] = set[8] = True;
  set[2] = set[6] = False;

  TEST_MSG("Conjunction between non-empty and empty");
  TritSet result = set & emptySet;
  for (size_t i = 0u; i < set.length(); ++i) {
    EXPECT_EQ((Trit)result[i], (Trit)set[i] & Unknown);
  }

  TEST_MSG("Extending empty set");
  emptySet[15] = True;
  EXPECT_EQ(emptySet.length(), 16u);
  EXPECT_EQ(emptySet.capacity(), 1u);
  EXPECT_EQ(emptySet[10], Unknown);
  EXPECT_EQ(emptySet[15], True);

  set = emptySet;

  SUCCESS_MSG;
}

TEST(TritSet, CopyAssignmentOperator) {

  START_MSG("CopyAssignmentOperator");

  TritSet A(10);
  TritSet B(10);

  A[0] = True;
  ASSERT_EQ(A[0], True);
  A[13] = False;
  ASSERT_EQ(A.length(), 14u);
  ASSERT_EQ(A[13], False);
  A[5] = False;
  A[13] = Unknown;
  ASSERT_EQ(A.length(), 6u);
  ASSERT_EQ(A[5], False);

  TritSet C = B;

  ASSERT_EQ(C.length(), B.length());
  ASSERT_EQ(C.capacity(), B.capacity());
  for (size_t i = 0; i < B.length(); ++i) {
    EXPECT_EQ(C[i], B[i]);
  }

  TEST_MSG("Copying into empty default-construted set");
  size_t lengthA = A.length();
  TritSet D;
  D = A;

  ASSERT_EQ(lengthA, A.length()) << "Assignment mustn't change original set";
  ASSERT_EQ(D.length(), A.length());
  ASSERT_EQ(D.capacity(), A.capacity());
  TEST_MSG("Checking element-wise equality")
  for (size_t i = 0; i < A.length(); ++i) {
    EXPECT_EQ(D[i], A[i]);
  }

  TEST_MSG("Copying 3 sets in a row");
  A = B = C;
  EXPECT_EQ(A.length(), B.length());
  EXPECT_EQ(C.length(), B.length());
  EXPECT_EQ(C.length(), A.length());

  for (size_t i = 0; i < A.length(); ++i) {
    EXPECT_EQ(A[i], B[i]);
    EXPECT_EQ(A[i], C[i]);
    EXPECT_EQ(B[i], C[i]);
  }

  SUCCESS_MSG;
}


TEST(TritSet, LogicOperators) {

  START_MSG("LogicOperators");

  TritSet A(10);
  TritSet B(10);
  TritSet C(20);

  A[3] = A[6] = A[7] = False;
  A[2] = A[4] = True;
  
  for (size_t i = 0; i < A.length(); ++i) {
    C[i] = A[i];
  }
  C[13] = C[17] = True;
  C[11] = C[12] = C[19] = False;

  B[3] = B[4] = B[8] = True;
  B[1] = B[6] = False;

  TEST_MSG("Check conjunction");
  TritSet D = A & B;

  EXPECT_EQ(D[0], Unknown);
  EXPECT_EQ(D[1], False);
  EXPECT_EQ(D[2], Unknown);
  EXPECT_EQ(D[3], False);
  EXPECT_EQ(D[4], True);
  EXPECT_EQ(D[5], Unknown);
  EXPECT_EQ(D[6], False);
  EXPECT_EQ(D[7], False);
  EXPECT_EQ(D[8], Unknown);
  EXPECT_EQ(D[9], Unknown);

  TEST_MSG("Check disjunction");
  D = A | B;

  EXPECT_EQ(D[0], Unknown);
  EXPECT_EQ(D[1], Unknown);
  EXPECT_EQ(D[2], True);
  EXPECT_EQ(D[3], True);
  EXPECT_EQ(D[4], True);
  EXPECT_EQ(D[5], Unknown);
  EXPECT_EQ(D[6], False);
  EXPECT_EQ(D[7], Unknown);
  EXPECT_EQ(D[8], True);
  EXPECT_EQ(D[9], Unknown);

  TEST_MSG("Check conjunction with length extension...");

  D = B & C;

  TEST_MSG("Assert length and capacity equality");

  ASSERT_EQ(D.length(), C.length());
  ASSERT_EQ(D.capacity(), C.capacity());

  TEST_MSG("Check operation correctness")

  EXPECT_EQ(D[0], Unknown);
  EXPECT_EQ(D[1], False);
  EXPECT_EQ(D[2], Unknown);
  EXPECT_EQ(D[3], False);
  EXPECT_EQ(D[4], True);
  EXPECT_EQ(D[5], Unknown);
  EXPECT_EQ(D[6], False);
  EXPECT_EQ(D[7], False);
  EXPECT_EQ(D[8], Unknown);
  EXPECT_EQ(D[9], Unknown);
  EXPECT_EQ(D[10], Unknown);
  EXPECT_EQ(D[11], False);
  EXPECT_EQ(D[12], False);
  EXPECT_EQ(D[13], Unknown);
  EXPECT_EQ(D[14], Unknown);
  EXPECT_EQ(D[15], Unknown);
  EXPECT_EQ(D[16], Unknown);
  EXPECT_EQ(D[17], Unknown);
  EXPECT_EQ(D[18], Unknown);
  EXPECT_EQ(D[19], False);

  TEST_MSG("Check operator&=");

  B &= C;

  TEST_MSG("Assert length and capacity equality");

  ASSERT_EQ(B.length(), C.length());
  ASSERT_EQ(B.capacity(), C.capacity());

  for (size_t i = 0; i < B.length(); ++i) {
    EXPECT_EQ(D[i], B[i]);
  }


  TEST_MSG("Check operator==");

  EXPECT_TRUE(A == A);
  EXPECT_TRUE(B == B);
  EXPECT_TRUE(B == D);
  EXPECT_FALSE(C == D);
  EXPECT_FALSE(B != D);
  EXPECT_FALSE(D != D);

  TEST_MSG("Check operator~");

  TritSet NotD = ~D;
  for (size_t i = 0; i < D.length(); ++i) {
    EXPECT_EQ(D[i], !NotD[i]);
  }

  SUCCESS_MSG;
}

TEST(TritSet, SizeAndCardinality) {

  START_MSG("SizeAndCardinality");

  TritSet A(10);
  TritSet B(10);
  TritSet C(20);

  A[3] = A[6] = A[7] = False;
  A[2] = A[4] = True;

  ASSERT_EQ(A.length(), 8u);
  ASSERT_EQ(A.capacity(), 1u);

  auto card = A.cardinality();
  ASSERT_EQ(card[True], 2u);
  ASSERT_EQ(card[False], 3u);
  ASSERT_EQ(card[Unknown], 3u);

  B[3] = B[4] = B[8] = True;
  B[1] = B[6] = False;

  ASSERT_EQ(B.length(), 9u);
  ASSERT_EQ(B.capacity(), 1u);

  card = B.cardinality();
  ASSERT_EQ(card[True], 3u);
  ASSERT_EQ(card[False], 2u);
  ASSERT_EQ(card[Unknown], 4u);

  for (size_t i = 0; i < A.length(); ++i) {
    C[i] = A[i];
  }

  C[13] = C[17] = True;
  C[11] = C[12] = C[19] = False;

  ASSERT_EQ(C.length(), 20u);
  ASSERT_EQ(C.capacity(), 2u);

  card = C.cardinality();
  ASSERT_EQ(card[True], 4u);
  ASSERT_EQ(card[False], 6u);
  ASSERT_EQ(card[Unknown], 10u);

  size_t cardinality = C.cardinality(True);

  EXPECT_EQ(cardinality, 4u);

  cardinality = C.cardinality(Unknown);
  EXPECT_EQ(cardinality, 10u);
  cardinality = C.cardinality(False);
  EXPECT_EQ(cardinality, 6u);

  TritSet D = A & ~C;

  ASSERT_EQ(D.length(), 18u);
  ASSERT_EQ(D.capacity(), 2u);

  card = D.cardinality();
  ASSERT_EQ(card[True], 0u);
  ASSERT_EQ(card[False], 7u);
  ASSERT_EQ(card[Unknown], 11u);

  C[17] = C[19] = Unknown;
  C.shrink();
  EXPECT_EQ(C.length(), 14u);

  C.trim(5);
  EXPECT_EQ(C.length(), 5u);
  EXPECT_EQ(C.capacity(), 1u);

  B.trim(6);
  EXPECT_EQ(C.length(), 5u);
  EXPECT_EQ(C.capacity(), 1u);

}

TEST(TritSet, Iterator) {
  START_MSG("Iterator");

  TritSet B(10);
  B[3] = B[4] = B[8] = True;
  B[1] = B[6] = False;

  int i = 0;
  for (auto it = B.begin(); it != B.end(); ++it) {
    if (i == 3 || i == 4 || i == 8) {
      EXPECT_EQ(*it, True);
    }
    else if (i == 1 || i == 6) {
      EXPECT_EQ(*it, False);
    }
    else {
      EXPECT_EQ(*it, Unknown);
    }
    i++;
  }

  for (auto& it : B) {
    it = True;
  }

  for (auto it : B) {
    EXPECT_EQ(it, True);
  }

  for (auto& it : B) {
    it = Unknown;
  }

  EXPECT_EQ(B.length(), 0u);
}

TEST(TritHolder, AssginmentAndLogicOperators) {

  START_MSG("Testing TritHolder");
  START_MSG("AssginmentAndLogicOperators");

  TritSet B(10);
  B[3] = B[4] = B[8] = True;
  B[1] = B[6] = False;

  ASSERT_TRUE(B[3] == True);
  ASSERT_TRUE(B[3] == B[4]);
  ASSERT_TRUE(B[3] == B[8]);
  ASSERT_TRUE(B[1] == False);
  ASSERT_TRUE(B[1] == B[6]);
  ASSERT_TRUE(B[1] != B[3]);
  ASSERT_FALSE(B[1] != B[6]);
  ASSERT_FALSE(B[4] != B[8]);

  B[3] = !B[4];
  ASSERT_TRUE(B[3] == False);
  ASSERT_TRUE(B[3] == B[1]);

  SUCCESS_MSG;
}

TEST(TritSet, NegativeTest) {

  START_MSG("NegativeTest");
  TritSet B(10);
  B[3] = B[4] = B[8] = True;
  B[1] = B[6] = False;

  auto it = B.end();
  it++;
  ASSERT_ANY_THROW(*it);

  SUCCESS_MSG;
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}