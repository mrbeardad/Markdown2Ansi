#include "utils/string.hpp"

#include <string>

#include "gtest/gtest.h"

// NOLINTNEXTLINE: gtest
TEST(UtilsString, NormalTest) {
  std::string str{};
  str << "cstr" << std::string{"string"} << 'c';
  EXPECT_EQ(str, "cstrstringc");
  EXPECT_EQ(str * 3, "cstrstringccstrstringccstrstringc");
  EXPECT_EQ(Repeat("cao", 3), "caocaocao");
}
