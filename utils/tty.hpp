#ifndef SEE_UTILS_TTY_HPP_
#define SEE_UTILS_TTY_HPP_

#include <sys/ioctl.h>
#include <string>

namespace see::utils {

using WinSize = ::winsize;

#define SEE_DEFAULT_WIN_HEIGHT 30
#define SEE_DEFAULT_WIN_WIDTH  80

auto GetTtyWinSize() -> WinSize;

auto DisplayWidth(const std::string& input) -> int;

auto DisplayWidth(const std::wstring& input) -> int;

}  // namespace see::utils

#endif  // !SEE_UTILS_TTY_HPP_
