#include "md2ansi/md2ansi.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

#include "gtest/gtest.h"

#include "project.hpp"

TEST(Md2AnsiMd2Ansi, HighlightTestNormal) {
    auto* filename = std::getenv("MD_FILE");
    if (filename == nullptr) {
        return;
    }
    if (filename == std::string{""}) {
        filename = TEST_CHEAT_DIR "/markdown.md";
    }
    std::ifstream mdfile{filename};
    std::string text{std::istreambuf_iterator<char>{mdfile},
                     std::istreambuf_iterator<char>{}};
    std::cout << see::md2ansi::Highlight(text) << std::endl;
}
