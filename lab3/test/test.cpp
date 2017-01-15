#include <gtest/gtest.h>
#include "../CSVParser.h"
#include "../TuplePrinter.h"

using namespace std;
#define CSV_DIR string("csv files")
#define WRITE(MSG) cout << (MSG) << endl;

TEST(Parser, OpeningFile) {
  WRITE("Trying to open non-existing file");

  ASSERT_ANY_THROW(CSVParser<string> parser("non-existing-file.txt"));
};

TEST(Parser, SimpleCSVFile1) {
  WRITE("Parsing csv files without escape characters");
  CSVParser<string, string, string> parser(CSV_DIR + "/simple_lines.csv");
  tuple<string, string, string> expected{"a", "b", "c"};

  ASSERT_EQ(expected, parser.getNextRow());
};

TEST(Parser, SimpleCSVFile2) {
  CSVParser<string, string, string> parser(CSV_DIR + "/read_whitespace.csv");
  tuple<string, string, string> expected{"a", " b", " c"};

  ASSERT_EQ(expected, parser.getNextRow());
};

TEST(Parser, SimpleCSVFile3) {
  CSVParser<string, double, int> parser(CSV_DIR + "/various_types.csv");
  tuple<string, double, int> expected{"a", 3.1415, 42};

  ASSERT_EQ(expected, parser.getNextRow());
};

TEST(Parser, SimpleCSVFile4) {
  CSVParser<string, double, int> parser(CSV_DIR + "/multiple_lines.csv");
  tuple<string, double, int> expected_line1{"csv ", 3.14, 1};
  tuple<string, double, int> expected_line2{"is ", 2.71, 2};
  tuple<string, double, int> expected_line3{"a great format", 0.67, 3};

  ASSERT_EQ(expected_line1, parser.getNextRow());
  ASSERT_EQ(expected_line2, parser.getNextRow());
  ASSERT_EQ(expected_line3, parser.getNextRow());
};

TEST(Parser, ComplexCSVFile1) {
  WRITE("Parsing csv files with escape characters");
  WRITE("Escaping newline character as a row-delimiter");

  CSVParser<int, string, string, string> parser(CSV_DIR + "/escape_newline.csv");
  tuple<int, string, string, string> expected{1997,"Ford","E350","Go get one now\nthey are going fast"};

  ASSERT_EQ(expected, parser.getNextRow());
}

TEST(Parser, ComplexCSVFile2) {
  WRITE("Escaping comma character as a column-delimiter");

  CSVParser<int, string, string, string> parser(CSV_DIR + "/escape_comma.csv");
  tuple<int, string, string, string> expected{1997,"Ford","E350","Super, luxurious truck"};

  ASSERT_EQ(expected, parser.getNextRow());
}

TEST(Parser, ComplexCSVFile3) {
  WRITE("Escaping \" as escape character");

  CSVParser<int, string, string, string> parser(CSV_DIR + "/escape_quotes.csv");
  tuple<int, string, string, string> expected{1997,"Ford","E350","Super, \"luxurious\" truck"};

  ASSERT_EQ(expected, parser.getNextRow());
}

TEST(Parser, ComplexCSVFile4) {
  WRITE("Parsing complex csv");

  CSVParser<string, double, string> parser(CSV_DIR + "/escape_complex.csv");
  tuple<string, double, string> expected{"Free trip to A,B", 5.89, "Special rate \"1.79\""};

  ASSERT_EQ(expected, parser.getNextRow());
  cout << parser.getCachedRow() << endl;
}

TEST(Parser, ComplexCSVFile5) {
  WRITE("Parsing complex csv");

  CSVParser<int, string, string,string, double> parser(CSV_DIR + "/escape_collections.csv");
  tuple<int, string, string, string, double> expected_line1{1997,	"Ford",	 "E350",                          "ac, abs, moon",	3000.00};
  tuple<int, string, string, string, double> expected_line2{1999, "Chevy", "Venture \"Extended Edition\"",  "",               4600.00};
  tuple<int, string, string, string, double> expected_line3{1997, "Chevy", "Venture \"Extended Edition, Very Large\"", "",    5000.00};
  tuple<int, string, string, string, double> expected_line4{1996, "Jeep",  "Grand Cherokee",                "MUST SELL!\nair, moon roof, loaded", 4799.00};


  ASSERT_EQ(expected_line1, parser.getNextRow());
  cout << parser.getCachedRow() << endl;
  ASSERT_EQ(expected_line2, parser.getNextRow());
  cout << parser.getCachedRow() << endl;
  ASSERT_EQ(expected_line3, parser.getNextRow());
  cout << parser.getCachedRow() << endl;
  ASSERT_EQ(expected_line4, parser.getNextRow());
  cout << parser.getCachedRow() << endl;
}

TEST(Parser, NegativeTest1) {
  WRITE("Test template types to not match csv types");
  CSVParser<int> parser(CSV_DIR + "/wrong_types1.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest2) {
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_types2.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest3) {
  WRITE("Test wrong line lengths");
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_line_length1.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest4) {
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_line_length2.csv");

  parser.getNextRow();
  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest5) {
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_line_length3.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest6) {
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_line_length4.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest7) {
  WRITE("Test wrong use of escape character");
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_escape1.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, NegativeTest8) {
  CSVParser<int, string, double> parser(CSV_DIR + "/wrong_escape2.csv");

  ASSERT_ANY_THROW(parser.getNextRow());
}

TEST(Parser, CustomControls1) {
  CSVParser<string, double, int> parser(CSV_DIR + "/custom_newline.csv", '\\');

  tuple<string, double, int> expected_line1{"csv ", 3.14, 1};
  tuple<string, double, int> expected_line2{"is ", 2.71, 2};
  tuple<string, double, int> expected_line3{"a great format", 0.67, 3};

  ASSERT_EQ(expected_line1, parser.getNextRow());
  ASSERT_EQ(expected_line2, parser.getNextRow());
  ASSERT_EQ(expected_line3, parser.getNextRow());
}

TEST(Parser, CustomControls2) {
  CSVParser<string, double, int> parser(CSV_DIR + "/custom_column.csv", '\n', ' ');

  tuple<string, double, int> expected_line1{"csv ", 3.14, 1};
  tuple<string, double, int> expected_line2{"is ", 2.71, 2};
  tuple<string, double, int> expected_line3{"a great format", 0.67, 3};

  ASSERT_EQ(expected_line1, parser.getNextRow());
  ASSERT_EQ(expected_line2, parser.getNextRow());
  ASSERT_EQ(expected_line3, parser.getNextRow());
}

TEST(Parser, CustomControls3) {
  CSVParser<string, double, int> parser(CSV_DIR + "/custom_escape.csv", '\n', ' ', '.');

  tuple<string, double, int> expected_line1{"csv ", 3.14, 1};
  tuple<string, double, int> expected_line2{"is ", 2.71, 2};
  tuple<string, double, int> expected_line3{"a great format", 0.67, 3};

  ASSERT_EQ(expected_line1, parser.getNextRow());
  ASSERT_EQ(expected_line2, parser.getNextRow());
  ASSERT_EQ(expected_line3, parser.getNextRow());
}

TEST(Parser, Iterator1) {
  WRITE("Test iterator");

  CSVParser<string, string, string> parser(CSV_DIR + "/simple_lines.csv");

  CSVIterator<string, string, string> it1 = parser.begin();
  CSVIterator<string, string, string> it2 = parser.begin();
  CSVIterator<string, string, string> it3 = parser.begin();

  EXPECT_EQ(*it1, *it2);
  EXPECT_EQ(*it2, *it3);
}

TEST(Parser, Iterator2) {
  CSVParser<string, string, string> parser(CSV_DIR + "/simple_lines.csv");

  CSVIterator<string, string, string> it1 = parser.end();

  EXPECT_THROW(*it1, out_of_range);
}

TEST(Parser, Iterator3) {
  CSVParser<string, string, string> parser(CSV_DIR + "/simple_lines.csv");

  CSVIterator<string, string, string> begin = parser.begin();
  CSVIterator<string, string, string> end = parser.end();

  for (; begin != end; begin++) {}

  EXPECT_THROW(*++begin, out_of_range);
}

TEST(Parser, Iterator4) {
  CSVParser<string, string, string> parser(CSV_DIR + "/simple_lines.csv");

  CSVIterator<string, string, string> begin = parser.begin();
  CSVIterator<string, string, string> end = parser.end();

  for (; begin != end; begin++) {}

  CSVIterator<string, string, string> begin1 = parser.begin();

  EXPECT_THROW(*++begin1, out_of_range);
}

TEST(Parser, CallExceptions) {
  CSVParser<string, double, int> parser(CSV_DIR + "/multiple_lines.csv");

  vector<tuple<string, double, int>> expected {
          tuple<string, double, int> {"csv ", 3.14, 1},
          tuple<string, double, int> {"is ", 2.71, 2},
          tuple<string, double, int> {"a great format", 0.67, 3}
  };

  size_t i = 0;
  for (const auto& tuple : parser) {
    ASSERT_EQ(expected[i], tuple);
    ++i;
  }
}


int main(int argc, char* argv[]) {
  cout << "RUNNING TESTS!!!" << endl;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}