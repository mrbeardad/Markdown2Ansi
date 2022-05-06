#include "utils/tty.hpp"

#include "gtest/gtest.h"

// NOLINTNEXTLINE: gtest
TEST(UtilsTty, DisplayWidthTestNormal) {
    EXPECT_EQ(see::utils::DisplayWidth("我叼你妈的"), 10);
    EXPECT_EQ(see::utils::DisplayWidth("\033nmsl"), 3);
    EXPECT_EQ(see::utils::DisplayWidth("\033[38;5;255mcnm"), 3);
}
