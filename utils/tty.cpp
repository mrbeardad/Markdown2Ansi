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
        ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &kWinSize);
    }

    return kWinSize;
}

auto DisplayWidth(const std::string& input) -> int {
    return DisplayWidth(unicode::details::utf8_decode(input));
}

auto DisplayWidth(const std::wstring& input) -> int {
    std::wstring str{};
    bool in_escape_sequence{false};
    for (size_t idx{0}; idx < input.length(); ++idx) {
        if (input[idx] == L'\033') {
            if (++idx < input.length() && input[idx] == L'[') {
                in_escape_sequence = true;
            }  // else discarde it
        } else if (in_escape_sequence) {
            if (std::iswalpha(input[idx])) {
                in_escape_sequence = false;
            }
        } else {
            str += input[idx];
        }
    }
    return unicode::display_width(str);
}

}  // namespace see::utils
