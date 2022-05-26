#include "md2ansi/md2ansi.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

#include "gtest/gtest.h"

#include "utils/project.hpp"

// NOLINTNEXTLINE: gtest
TEST(Md2AnsiMd2Ansi, HighlightTestNormal) {
  const auto* filename = std::getenv("TEST_MD_FILE");
  if (filename == nullptr) {
    std::cout << "set TEST_MD_FILE to enable test md2ansi." << std::endl;
    return;
  }
  if (filename == std::string{""}) {
    filename = TEST_CHEAT_DIR "/markdown.md";
  }
  std::ifstream mdfile{filename};
  std::string text{std::istreambuf_iterator<char>{mdfile}, std::istreambuf_iterator<char>{}};
  std::cout << see::md2ansi::SetupMd2Ansi().Highlight(text) << std::endl;
}
