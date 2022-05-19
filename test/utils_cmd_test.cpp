#include "utils/cmd.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "utils/project.hpp"

// NOLINTNEXTLINE: gtest
TEST(UtilsCmd, ParseCommandArgs) {
  std::vector<const char*> argv{};
  see::utils::CmdArgs cmd_args{};
  auto& files = cmd_args.files;
  auto& regexes = cmd_args.regexes;
  auto& disable_pager = cmd_args.disable_pager;
  auto& status = cmd_args.return_status;
  std::vector<std::filesystem::path> expect_files{};
  std::vector<std::string> exceptRegexes{};

  auto parse_command_args = [&]() {
    ::optind = 0;
    files.clear();
    regexes.clear();
    disable_pager = false;
    status = see::utils::kParseSuccess;
    cmd_args = see::utils::ParseCommandArgs(static_cast<int>(argv.size()),
                                            // NOLINTNEXTLINE: it don't modify string pointing to
                                            const_cast<char**>(argv.data()), true);
  };

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
  expect_files.assign(
      {TEST_CHEAT_DIR "/react.md", TEST_CHEAT_DIR "/redis.md", TEST_CHEAT_DIR "/regex.md"});
  std::sort(files.begin(), files.end());
  std::sort(expect_files.begin(), expect_files.end());
  EXPECT_EQ(status, see::utils::kParseSuccess);
  EXPECT_EQ(files, expect_files);

  argv.assign({"see", "-frea", "-fdev", "regex"});
  parse_command_args();
  expect_files.assign({TEST_CHEAT_DIR "/react.md", TEST_CHEAT_DIR "/devtool.md"});
  std::sort(files.begin(), files.end());
  std::sort(expect_files.begin(), expect_files.end());
  EXPECT_EQ(status, see::utils::kParseSuccess);
  EXPECT_EQ(files, expect_files);

  argv.assign({"see", "-fr", "-frea", "regex"});
  parse_command_args();
  expect_files.assign(
      {TEST_CHEAT_DIR "/react.md", TEST_CHEAT_DIR "/regex.md", TEST_CHEAT_DIR "/redis.md"});
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

// NOLINTNEXTLINE: gtest
TEST(UtilsCmd, SearchEntries) {
  std::vector<std::filesystem::path> files{};
  std::vector<std::string> regexes{};
  std::string text{};
  std::string expect_text{};
  std::ifstream file_has_expect_text{};

  files.assign({TEST_CHEAT_DIR "/devtool.md"});
  regexes.assign({"curl"});
  text = see::utils::SearchEntries(files, regexes);
  file_has_expect_text.open(TEST_CHEAT_DIR "/expect_devtool_curl.txt");
  expect_text.assign(std::istreambuf_iterator<char>{file_has_expect_text},
                     std::istreambuf_iterator<char>{});
  expect_text = see::utils::kFileNameStartTag + files[0].string() + see::utils::kFileNameEndTag
                + '\n' + expect_text;
  EXPECT_EQ(text, expect_text);

  files.assign({TEST_CHEAT_DIR "/devtool.md"});
  regexes.assign({"curl|gcc"});
  text = see::utils::SearchEntries(files, regexes);
  file_has_expect_text.close();
  file_has_expect_text.clear();
  file_has_expect_text.open(TEST_CHEAT_DIR "/expect_devtool_curl_gcc.txt");
  expect_text.assign(std::istreambuf_iterator<char>{file_has_expect_text},
                     std::istreambuf_iterator<char>{});
  expect_text = see::utils::kFileNameStartTag + files[0].string() + see::utils::kFileNameEndTag
                + '\n' + expect_text;
  EXPECT_EQ(text, expect_text);

  files.assign({TEST_CHEAT_DIR "/devtool.md", TEST_CHEAT_DIR "/regex.md"});
  regexes.assign({"es"});
  text = see::utils::SearchEntries(files, regexes);
  file_has_expect_text.close();
  file_has_expect_text.clear();
  file_has_expect_text.open(TEST_CHEAT_DIR "/expect_devtool_es.txt");
  expect_text.assign(std::istreambuf_iterator<char>{file_has_expect_text},
                     std::istreambuf_iterator<char>{});
  expect_text = see::utils::kFileNameStartTag + files[0].string() + see::utils::kFileNameEndTag
                + '\n' + expect_text + see::utils::kFileNameStartTag + files[1].string()
                + see::utils::kFileNameEndTag + '\n';
  file_has_expect_text.close();
  file_has_expect_text.clear();
  file_has_expect_text.open(TEST_CHEAT_DIR "/expect_regex_es.txt");
  std::string str{std::istreambuf_iterator<char>{file_has_expect_text},
                  std::istreambuf_iterator<char>{}};
  expect_text += str;
  EXPECT_EQ(text, expect_text);
}
