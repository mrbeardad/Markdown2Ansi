#include "cmd.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <ostream>
#include <regex>
#include <string>
#include <system_error>
#include <tuple>
#include <vector>

#include "boost/algorithm/string/predicate.hpp"

#include "utils/string.hpp"

namespace see::utils {

namespace {

const auto kHelpInformation = R"(
Usage:

    see [-p] [-f <file-prefix>] <regex-words>...
    cat text.md | see

Search cheat sheet in ~/.cheat/*.md, each entry starts with '<!-- entry begin: keywords -->'
and ends with '<!-- entry end -->', <regex-words> in command line will match keywords,
and then print them with highlight. If see read from pipe, it will highlight the text read
from pipe instead of entries searching in cheat sheets.

Options:
    -h                  = Print this help information
    -p                  = Don't use pager even if the highlighted text if too width or too long
    -f <file-prefix>    = Specify prefix of filename in ~/.cheat to search entries
)";

void PrintErrorInformation(int status, bool silent = false, const std::string& info = "") {
  if (silent) {
    return;
  }
  switch (status) {
    case kParseError:
      std::cerr << "see: Command line arguments error: " + info + "\n\n" << kHelpInformation;
      break;
    case kFoundNoFile:
      std::cerr << "see: Found no file: " + info + '\n';
      break;
    case kHelpOption:
      std::cout << kHelpInformation << std::endl;
      break;
    default:
      break;
  }
}

auto GetCheatDir() -> std::filesystem::path {
  auto dirname = std::getenv("SEE_CHEAT_DIR");
  std::filesystem::path cheat_dir{dirname == nullptr ? "" : dirname};
  if (cheat_dir.empty()) {
    cheat_dir = std::getenv("HOME");
    cheat_dir /= ".cheat";
  }
  return cheat_dir;
}

}  // namespace

auto ParseCommandArgs(int argc, char** argv, bool silent) -> CmdArgs {
  CmdArgs cmd_args{};
  std::vector<std::string> file_prefixes{};

  for (;;) {
    auto choice = ::getopt(argc, argv, "hpf:");

    if (choice == -1) {
      break;
    }

    switch (choice) {
      case 'h':
        PrintErrorInformation(kHelpOption, silent);
        cmd_args.return_status = kHelpOption;
        return cmd_args;
      case 'p':
        cmd_args.disable_pager = true;
        break;
      case 'f':
        file_prefixes.emplace_back(optarg);
        break;
      case '?':
        PrintErrorInformation(kParseError, silent);
        cmd_args.return_status = kParseError;
        return cmd_args;
      default:
        break;
    }
  }

  while (::optind < argc) {
    cmd_args.regexes.emplace_back(argv[::optind++]);
  }

  if (cmd_args.regexes.empty()) {
    PrintErrorInformation(kParseError, silent, "At least one regex word to search for entries.");
    cmd_args.return_status = kParseError;
  }

  auto cheat_dir = GetCheatDir();

  // if didn't specify files, search for all files
  std::error_code dir_errcode{};
  auto cheat_dir_entries = std::filesystem::directory_iterator{cheat_dir, dir_errcode};
  if (dir_errcode) {
    PrintErrorInformation(kFoundNoFile, silent, dir_errcode.message());
    cmd_args.return_status = kFoundNoFile;
    return cmd_args;
  }
  if (file_prefixes.empty()) {
    std::copy_if(cheat_dir_entries, std::filesystem::directory_iterator{},
                 std::back_inserter(cmd_args.files),
                 [](const auto& cur_file) { return cur_file.path().extension() == ".md"; });
  } else {
    for (const auto& cur_direntry : cheat_dir_entries) {
      if (!std::filesystem::is_regular_file(cur_direntry)
          || cur_direntry.path().extension() != ".md") {
        continue;
      }

      std::string cur_filename{cur_direntry.path().filename()};
      for (auto& cur_file_prefix : file_prefixes) {
        if (cur_filename.find(cur_file_prefix) == 0) {
          cmd_args.files.emplace_back(cur_direntry.path());
          break;
        }
      }
    }
  }

  if (cmd_args.files.empty()) {
    PrintErrorInformation(kFoundNoFile, silent);
    cmd_args.return_status = kFoundNoFile;
  }

  return cmd_args;
}

namespace {

const auto kEntryBeginPrefix = "<!-- entry begin:";
const auto kEntryBeginSuffix = "-->";
const auto kEntryEnd = "<!-- entry end -->";

auto FileNameTag(const std::string& filename) -> std::string {
  return kFileNameStartTag + filename + kFileNameEndTag + '\n';
}

}  // namespace

auto SearchEntries(const std::vector<std::filesystem::path>& files,
                   const std::vector<std::string>& regexes) -> std::string {
  std::string entries{};

  std::string entries_in_cur_file{};
  for (const auto& cur_file : files) {
    std::ifstream fstrm{cur_file, std::ios_base::in};
    if (!fstrm) {
      std::cerr << "see: cannot access '" << cur_file.string() << "': Permission denied\n";
      return entries;
    }

    bool is_in_entry{false};
    for (std::string oneline{}; std::getline(fstrm, oneline);) {
      if (!is_in_entry && boost::starts_with(oneline, kEntryBeginPrefix)
          && boost::ends_with(oneline, kEntryBeginSuffix)) {
        // catch entry begin
        auto is_match
            = std::all_of(regexes.begin(), regexes.end(), [&oneline](const auto& cur_regex) {
                return std::regex_search(
                    oneline.begin() + static_cast<ptrdiff_t>(std::strlen(kEntryBeginPrefix)),
                    oneline.end() - static_cast<ptrdiff_t>(std::strlen(kEntryBeginSuffix)),
                    std::regex{cur_regex});
              });

        if (is_match) {
          is_in_entry = true;
          entries_in_cur_file << '\n';
        }
      } else if (is_in_entry) {
        // catch entry content or entry end
        if (oneline != kEntryEnd) {
          entries_in_cur_file << oneline << '\n';
        } else {
          is_in_entry = false;
          entries_in_cur_file << '\n';
        }
      }
    }
    if (!entries_in_cur_file.empty()) {
      entries << FileNameTag(cur_file) << entries_in_cur_file;
    }
    entries_in_cur_file.clear();
  }

  return entries;
}

auto ExecPager() -> int {
  const auto* pager = ::getenv("PAGER");
  pager = pager != nullptr ? pager : "less";
  if (pager == std::string{"less"}) {
    return ::execlp("less", "less", "-Si", nullptr);
  }
  return ::execlp(pager, pager, nullptr);
}

}  // namespace see::utils
