#include "utils/cmd.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "project.hpp"

// NOLINTNEXTLINE: allowed gtest
TEST(UtilsCmd, ParseCommandArgsTestAll) {
    int status{};
    std::vector<const char*> argv{};
    std::vector<std::filesystem::path> files{};
    std::vector<std::filesystem::path> expect_files{};
    std::vector<std::string> regexes{};
    std::vector<std::string> exceptRegexes{};
    bool disable_pager{};

    auto parse_command_args = [&]() {
        ::optind = 0;
        files.clear();
        regexes.clear();
        disable_pager = false;
        status = see::utils::ParseCommandArgs(
            static_cast<int>(argv.size()),
            // NOLINTNEXTLINE: is ok to cast const char** to char**
            const_cast<char**>(argv.data()), files, regexes, disable_pager, 1);
    };

#define TEST_CHEAT_DIR PROJECT_SOURCE_DIR "/test/test_cheat"
    ::setenv("SEE_CHEAT_DIR", TEST_CHEAT_DIR, 1);

    argv.assign({});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseError);
    EXPECT_EQ(regexes.size(), 0);

    argv.assign({""});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseError);
    EXPECT_EQ(regexes.size(), 0);

    argv.assign({"see"});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseError);
    EXPECT_EQ(regexes.size(), 0);

    argv.assign({"see", ""});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(regexes.size(), 1);

    argv.assign({"see", "regex"});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(regexes.size(), 1);

    argv.assign({"see", "regex1", "regex2"});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(regexes.size(), 2);

    argv.assign({"see", "-f", "regex", "regex"});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(files.size(), 1);

    argv.assign({"see", "-f", "r", "regex"});
    parse_command_args();
    expect_files.assign({TEST_CHEAT_DIR "/react.md", TEST_CHEAT_DIR "/redis.md",
                         TEST_CHEAT_DIR "/regex.md"});
    std::sort(files.begin(), files.end());
    std::sort(expect_files.begin(), expect_files.end());
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(files, expect_files);

    argv.assign({"see", "-frea", "-fdev", "regex"});
    parse_command_args();
    expect_files.assign(
        {TEST_CHEAT_DIR "/react.md", TEST_CHEAT_DIR "/devtool.md"});
    std::sort(files.begin(), files.end());
    std::sort(expect_files.begin(), expect_files.end());
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(files, expect_files);

    argv.assign({"see", "-fr", "-frea", "regex"});
    parse_command_args();
    expect_files.assign({TEST_CHEAT_DIR "/react.md", TEST_CHEAT_DIR "/regex.md",
                         TEST_CHEAT_DIR "/redis.md"});
    std::sort(files.begin(), files.end());
    std::sort(expect_files.begin(), expect_files.end());
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(files, expect_files);

    argv.assign({"see", "-f", "unknown", "regex"});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kFoundNoFile);
    EXPECT_EQ(files.size(), 0);

    argv.assign({"see", "-p", "regex"});
    parse_command_args();
    EXPECT_EQ(status, see::utils::kParseSuccess);
    EXPECT_EQ(disable_pager, true);
}
