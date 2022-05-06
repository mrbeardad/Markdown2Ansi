#include "md2ansi/block_html.hpp"

#include <cstddef>
#include <cstring>
#include <string>

#include "boost/algorithm/string/predicate.hpp"

#include "utils/cmd.hpp"
#include "utils/color.hpp"
#include "utils/string.hpp"
#include "utils/tty.hpp"

namespace see::md2ansi {

namespace {

auto HighlightFilename(const std::string& filename) -> std::string {
    auto win_size = utils::GetTtyWinSize();
    // Except 2 space around filename
    auto remain_length =
        static_cast<size_t>(win_size.ws_col) - filename.length() - 2;

    std::string filename_header{};
    for (size_t i{}; i < remain_length / 2; ++i) {
        filename_header << "─";
    }
    filename_header << ' ' << filename << ' ';
    for (size_t i{}; i < remain_length - remain_length / 2; ++i) {
        filename_header << "─";
    }
    return utils::RainbowText(filename_header);
}

}  // namespace

auto HandleHtmlStart(std::string& /* text */) -> void {}

auto HandleHtmlEnd(std::string& /* text */) -> void {}

auto HandleHtmlText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len)
    -> void {
    static size_t kFilenameStartPos{};
    std::string raw(raw_text, len);
    if (raw == utils::kFileNameStartTag) {
        kFilenameStartPos = text.size();
    } else if (raw == utils::kFileNameEndTag) {
        std::string filename{text.substr(kFilenameStartPos)};
        text.erase(kFilenameStartPos);
        text += HighlightFilename(filename);
    }
}

}  // namespace see::md2ansi
