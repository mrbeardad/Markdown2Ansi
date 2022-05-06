#include "md2ansi/block_hr.hpp"
#include <string>
#include "utils/tty.hpp"

namespace see::md2ansi {

const int kIndentCount = 2;

auto HandleHrStart(std::string& text) -> void {
    auto winsize = utils::GetTtyWinSize();
    auto horline_len = winsize.ws_col - kIndentCount * 2;
    text += std::string(kIndentCount, ' ');
    text += "\033[35;1m";
    for (auto i = 0; i < horline_len; ++i) {
        text.append("─");
    }
    text += "\033[m";
}

auto HandleHrEnd(std::string& text) -> void {
    text += '\n';
}

}  // namespace see::md2ansi
