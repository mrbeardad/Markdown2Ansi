#include "md2ansi/html.hpp"
#include <sys/types.h>

#include <cstddef>
#include <cstring>
#include <string>

#include "md2ansi/md2ansi_text.hpp"
#include "md4c.h"
#include "utils/cmd.hpp"
#include "utils/color.hpp"
#include "utils/string.hpp"
#include "utils/tty.hpp"

namespace see::md2ansi {

namespace {

auto HighlightFilename(const std::string& filename) -> std::string {
  auto win_size = utils::GetTtyWinSize();
  // Except 2 space around filename
  auto remain_length = static_cast<size_t>(win_size.ws_col) - filename.length() - 2;

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

HtmlText::HtmlText() : TextHandler{MD_TEXT_HTML} {}

auto HtmlText::HandleText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len) -> void {
  static ssize_t kFilenameStartPos{-1};
  std::string raw(raw_text, len);
  if (raw == utils::kFileNameStartTag) {
    kFilenameStartPos = static_cast<ssize_t>(text.size());
  } else if (kFilenameStartPos != -1 && raw == utils::kFileNameEndTag) {
    std::string filename{text.substr(kFilenameStartPos)};
    text.erase(kFilenameStartPos);
    text += HighlightFilename(filename);
    kFilenameStartPos = -1;
  }
}

}  // namespace see::md2ansi
