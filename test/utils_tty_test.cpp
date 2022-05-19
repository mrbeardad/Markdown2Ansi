#include "utils/tty.hpp"

#include "gtest/gtest.h"

// NOLINTNEXTLINE: gtest
TEST(UtilsTty, DisplayWidthTestNormal) {
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的"), 10);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的nmsl"), 14);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的nmsl🤡"), 16);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的\rnmsl"), 10);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的\rnms"), 10);
  EXPECT_EQ(see::utils::DisplayWidth("nmsl\r我叼你妈的"), 10);
  EXPECT_EQ(see::utils::DisplayWidth("nmsl\r我叼你妈的"), 10);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的\bnmsl"), 13);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的\b\bnmsl"), 12);
  EXPECT_EQ(see::utils::DisplayWidth("nmsl\b我叼你妈的"), 13);
  EXPECT_EQ(see::utils::DisplayWidth("nmsl\b\b我叼你妈的"), 12);
  EXPECT_EQ(see::utils::DisplayWidth("nmsl\t我叼你妈的"), 18);
  EXPECT_EQ(see::utils::DisplayWidth("\t我叼你妈的"), 18);
  EXPECT_EQ(see::utils::DisplayWidth("nmslnm$l\t我叼你妈的"), 26);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的\tnmsl"), 20);
  EXPECT_EQ(see::utils::DisplayWidth("我叼你m\tnmsl"), 12);
  EXPECT_EQ(see::utils::DisplayWidth("nm\033sl"), 3);
  EXPECT_EQ(see::utils::DisplayWidth("nm\033[38;5;255sl"), 3);
  EXPECT_EQ(see::utils::DisplayWidth("n\033[38;5;255msl"), 3);
  EXPECT_EQ(see::utils::DisplayWidth("n\033\033[38;5;250msl"), 3);
  EXPECT_EQ(see::utils::DisplayWidth("n\033[38;5;\033250msl"), 6);
}
