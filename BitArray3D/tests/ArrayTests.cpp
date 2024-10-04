#include <lib/Numbers.h>
#include <gtest/gtest.h>
#include <sstream>

TEST(ArrayTests, PrimeNumber) {
  Numbers array = Numbers::MakeArray(5, 5, 5);
  array[1][1][1] = 100;
  ASSERT_EQ(array[1][1][1], 100);
}

TEST(ArrayTests, MoreThan16Bit) {
  Numbers array = Numbers::MakeArray(5, 5, 5);
  array[1][1][1] = 99923;
  ASSERT_EQ(array[1][1][1], 99923);
}

TEST(ArrayTests, Multi) {
  Numbers array = Numbers::MakeArray(5, 5, 5);
  int temp = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        array[i][j][m] = temp++;
      }
    }
  }
  Numbers new_arr = array * 10;
  bool res = true;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        ASSERT_EQ(array[i][j][m] * 10, new_arr[i][j][m]);
      }
    }
  }
  ASSERT_EQ(res, true);
}

TEST(ArrayTests, Minus) {
  Numbers array = Numbers::MakeArray(5, 5, 5);
  int temp = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        array[i][j][m] = temp++;
      }
    }
  }
  Numbers new_arr = array - array;
  bool res = true;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        ASSERT_EQ(new_arr[i][j][m], 0);
      }
    }
  }
  ASSERT_EQ(res, true);
}

TEST(ArrayTests, Plus) {
  Numbers array = Numbers::MakeArray(5, 5, 5);
  int temp = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        array[i][j][m] = temp++;
      }
    }
  }
  Numbers new_arr = array + array;
  bool res = true;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        ASSERT_EQ(new_arr[i][j][m],(array[i][j][m] + array[i][j][m]));
      }
    }
  }
  ASSERT_EQ(res, true);
}

TEST(ArrayTests, СinPerElement) {
  Numbers array = Numbers::MakeArray(2, 2, 2);
  std::string s = "1 2 3 4 5 6 7 8";
  std::istringstream input(s);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int m = 0; m < 2; ++m) {
        input >> array[i][j][m];
      }
    }
  }
  std::ostringstream os;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int m = 0; m < 2; ++m) {
        os << array[i][j][m];
      }
    }
  }
  ASSERT_EQ(os.str(), s);
}

TEST(ArrayTests, СinAllArray) {
  Numbers array = Numbers::MakeArray(2, 2, 2);
  std::string s = "1 2 3 4 5 6 7 8";
  std::istringstream input(s);
  input >> array;

  std::ostringstream os;
  os << array;
  ASSERT_EQ(os.str(), s);
}

TEST(ArrayTests, CopyTest1) {
  Numbers array = Numbers::MakeArray(2, 2, 2);
  std::string s = "1 2 3 4 5 6 7 8";
  std::istringstream input(s);
  input >> array;
  Numbers new_array = array;
  new_array[0][0][0] = new_array[0][0][0] - 1;
  ASSERT_EQ(array[0][0][0], 1);
}

TEST(ArrayTests, CopyTest2) {
  Numbers array = Numbers::MakeArray(2, 2, 2);
  std::string s = "1 2 3 4 5 6 7 8";
  std::istringstream input(s);
  input >> array;
  Numbers new_array = array;
  new_array[0][0][0] = new_array[0][0][0] - 1;
  ASSERT_EQ(new_array[0][0][0], 0);
}

TEST(ArrayTests, UB) {
  Numbers array1 = Numbers::MakeArray(2, 2, 2);
  std::string s1 = "1 2 3 4 5 6 7 8";
  std::istringstream input1(s1);
  input1 >> array1;
  Numbers array2 = Numbers::MakeArray(1, 2, 4);
  std::string s2 = "1 2 3 4 5 6 7 8";
  std::istringstream input2(s2);
  input2 >> array2;

  EXPECT_EXIT((array1 + array2), testing::ExitedWithCode(EXIT_FAILURE),"");
}

TEST(ArrayTests, IndexOutOfRange) {
  Numbers array1 = Numbers::MakeArray(2, 2, 2);
  std::string s1 = "1 2 3 4 5 6 7 8";
  std::istringstream input1(s1);
  input1 >> array1;

  EXPECT_EXIT(array1[1][2][3], testing::ExitedWithCode(EXIT_FAILURE),"");
}
