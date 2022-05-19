#include "md2ansi/hr.hpp"

#include <string>

#include "md2ansi/md2ansi_block.hpp"
#include "md4c.h"
#include "utils/tty.hpp"

namespace see::md2ansi {

const int kIndentCount = 2;

HrBlock::HrBlock() : BlockHandler(MD_BLOCK_HR, "\033[35m") {}

auto HrBlock::BlockStart(std::string& text, void* /*detail*/) -> void {
  auto winsize = utils::GetTtyWinSize();
  auto horline_len = winsize.ws_col - kIndentCount * 2;
  text += std::string(kIndentCount, ' ') + GetStyle();
  for (auto i = 0; i < horline_len; ++i) {
    text.append("─");
  }
}

auto HrBlock::BlockEnd(std::string& text, void* /*detail*/) -> void {
  text += '\n';
}

}  // namespace see::md2ansi
