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

#include "utils/color.hpp"
#include "utils/string.hpp"
#include "utils/sys.hpp"
#include "utils/tty.hpp"

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

void PrintErrorInformation(int status,
                           bool silent_mode = false,
                           const std::string& info = "") {
    if (silent_mode) {
        return;
    }
    switch (status) {
        case kParseError:
            std::cerr << "see: Command line arguments error: " + info + "\n\n"
                      << kHelpInformation;
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
    std::filesystem::path cheat_dir{std::getenv("SEE_CHEAT_DIR")};
    if (cheat_dir.empty()) {
        cheat_dir = std::getenv("HOME");
        cheat_dir /= ".cheat";
    }
    return cheat_dir;
}

}  // namespace

auto ParseCommandArgs(int argc,
                      char** argv,
                      std::vector<std::filesystem::path>& orig_files,
                      std::vector<std::string>& orig_regexes,
                      bool& orig_disable_pager,
                      int silent_mode) -> int {
    std::vector<std::filesystem::path> files{};
    std::vector<std::string> regexes{};
    bool disable_pager{};
    std::vector<std::string> file_prefixes{};
    for (;;) {
        auto choice = ::getopt(argc, argv, "hpf:");

        if (choice == -1) {
            break;
        }

        switch (choice) {
            case 'h':
                PrintErrorInformation(kHelpOption, silent_mode);
                return kHelpOption;
            case 'p':
                disable_pager = true;
                break;
            case 'f':
                file_prefixes.emplace_back(optarg);
                break;
            case '?':
                PrintErrorInformation(kParseError, silent_mode);
                return kParseError;
            default:
                break;
        }
    }

    while (::optind < argc) {
        regexes.emplace_back(argv[::optind++]);
    }

    if (regexes.empty()) {
        PrintErrorInformation(kParseError, silent_mode,
                              "At least one regex word to search for entries.");
        return kParseError;
    }

    auto cheat_dir = GetCheatDir();

    // if didn't specify files, search for all files
    std::error_code dir_errcode{};
    auto cheat_dir_entries =
        std::filesystem::directory_iterator{cheat_dir, dir_errcode};
    if (dir_errcode) {
        PrintErrorInformation(kFoundNoFile, silent_mode, dir_errcode.message());
        return kFoundNoFile;
    }
    if (file_prefixes.empty()) {
        std::copy_if(cheat_dir_entries, std::filesystem::directory_iterator{},
                     std::back_inserter(files), [](const auto& cur_file) {
                         return cur_file.path().extension() == ".md";
                     });
    } else {
        for (const auto& cur_direntry : cheat_dir_entries) {
            if (!std::filesystem::is_regular_file(cur_direntry) ||
                cur_direntry.path().extension() != ".md") {
                continue;
            }

            std::string cur_filename{cur_direntry.path().filename()};
            for (auto& cur_file_prefix : file_prefixes) {
                if (cur_filename.find(cur_file_prefix) == 0) {
                    files.emplace_back(cur_direntry.path());
                    break;
                }
            }
        }
    }

    if (files.empty()) {
        PrintErrorInformation(kFoundNoFile, silent_mode);
        return kFoundNoFile;
    }

    orig_files = std::move(files);
    orig_regexes = std::move(regexes);
    orig_disable_pager = disable_pager;
    return kParseSuccess;
}

namespace {

const auto kEntryBeginPrefix = "<!-- entry begin:";
const auto kEntryBeginSuffix = "-->";
const auto kEntryEnd = "<!-- entry end -->";
const auto kFileNameStartTag = "<header>";
const auto kFileNameEndTag = "</header>";

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
            std::cerr << "see: cannot access '" << cur_file.string()
                      << "': Permission denied\n";
            return entries;
        }

        bool is_in_entry{false};
        for (std::string oneline{}; std::getline(fstrm, oneline);) {
            if (!is_in_entry &&
                boost::starts_with(oneline, kEntryBeginPrefix) &&
                boost::ends_with(oneline, kEntryBeginSuffix)) {
                // catch entry begin
                auto is_match = std::all_of(
                    regexes.begin(), regexes.end(),
                    [&oneline](const auto& cur_regex) {
                        return std::regex_search(
                            oneline.begin() +
                                static_cast<ptrdiff_t>(
                                    std::strlen(kEntryBeginPrefix)),
                            oneline.end() - static_cast<ptrdiff_t>(
                                                std::strlen(kEntryBeginSuffix)),
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
    }

    return entries;
}

namespace {

const std::array kDefultPagers{"less", "more"};

}

auto GetPager() -> std::string {
    std::string pager{};
    std::vector default_pagers(kDefultPagers.begin(), kDefultPagers.end());
    if (auto* user_pager = ::getenv("PAGER"); user_pager != nullptr) {
        default_pagers.assign({user_pager});
    }

    auto stdout_copy = HandleSyscall(::dup(STDOUT_FILENO));
    auto stderr_copy = HandleSyscall(::dup(STDERR_FILENO));
    auto null_fd = HandleSyscall(::open("/dev/null", O_WRONLY));
    HandleSyscall(dup2(null_fd, STDOUT_FILENO));
    HandleSyscall(dup2(null_fd, STDERR_FILENO));

    for (const auto* this_pager : default_pagers) {
        if (::execlp(this_pager, this_pager, "--version", nullptr) < 0) {
            pager = this_pager;
            break;
        }
    }

    HandleSyscall(::dup2(stdout_copy, STDOUT_FILENO));
    HandleSyscall(::dup2(stderr_copy, STDERR_FILENO));
    HandleSyscall(::close(null_fd));

    return pager;
}

}  // namespace see::utils
