#include "utils/tty.hpp"

#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include "unicode/display_width.hpp"

namespace see::utils {

auto GetTtyWinSize() -> WinSize {
  static ::winsize kWinSize{};
  static bool kIsInited{false};

  if (!kIsInited) {
    kIsInited = true;
    if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &kWinSize) < 0) {
      kWinSize.ws_row = SEE_DEFAULT_WIN_HEIGHT;
      kWinSize.ws_col = SEE_DEFAULT_WIN_WIDTH;
    }
  }

  return kWinSize;
}

auto DisplayWidth(const std::string& input) -> int {
  return unicode::display_width(input);
}

auto DisplayWidth(const std::wstring& input) -> int {
  return unicode::display_width(input);
}

}  // namespace see::utils
