#ifndef SEE_UTILS_CMD_HPP_
#define SEE_UTILS_CMD_HPP_

#include <filesystem>
#include <string>
#include <vector>

namespace see::utils {

enum {
    kParseSuccess,
    kHelpOption,
    kParseError,
    kFoundNoFile,
};

auto ParseCommandArgs(int argc,
                      char** argv,
                      std::vector<std::filesystem::path>& files,
                      std::vector<std::string>& regexes,
                      bool& disable_pager,
                      int silent_mode = 0) -> int;

auto SearchEntries(const std::vector<std::filesystem::path>& files,
                   const std::vector<std::string>& regexes) -> std::string;

auto GetPager() -> std::string;

}  // namespace see::utils

#endif  // !SEE_UTILS_CMD_HPP_
