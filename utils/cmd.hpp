#ifndef SEE_UTILS_CMD_HPP_
#define SEE_UTILS_CMD_HPP_

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace see::utils {

enum CmdParseStatus {
  kParseSuccess,
  kHelpOption,
  kParseError,
  kFoundNoFile,
};

struct CmdArgs {
  std::vector<std::filesystem::path>
      files;  ///< Files to search entries. By default search for all files in ~/.cheat directory,
              ///< you can specify environment variable SEE_CHEAT_DIR to change the default search
              ///< dirctory.
  std::vector<std::string>
      regexes;  ///< User specified regex expression to match entries. Give at least one regex.
  bool disable_pager;  ///< Disable pager. Launch pager when output is too long or too wide by
                       ///< default.
  CmdParseStatus return_status;  ///< Parsing status.
};

/**
 * @brief Parse command line arguments
 *
 * @param argc The first argument delivered to main()
 * @param argv The second argument delivered to main()
 * @param silent Disable output. Print error message when get error by default.
 * @return CmdArgs {files, regexes, disable_pager, return_status}.
 */
auto ParseCommandArgs(int argc, char** argv, bool silent = false) -> CmdArgs;

constexpr auto kFileNameStartTag = "<header>";
constexpr auto kFileNameEndTag = "</header>";

/**
 * @brief Search entries from the given files
 *
 * @param files Files for searching
 * @param regexes Used to match certain entries
 * @return std::string Return all entries found. File name will be inserted before the first entry
 * from that file. Each file name wraps with kFileNameStartTag and kFileNameEndTag
 */
auto SearchEntries(const std::vector<std::filesystem::path>& files,
                   const std::vector<std::string>& regexes) -> std::string;

/**
 * @brief Call exec to a pager program.
 *
 * Default pager is /usr/bin/less. When PAGER environment variable is set, exec pager specified by
 * PAGER.
 *
 * @return int Return value that function exec have returned.
 */
auto ExecPager() -> int;

}  // namespace see::utils

#endif  // !SEE_UTILS_CMD_HPP_
