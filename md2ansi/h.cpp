#include "md2ansi/h.hpp"

#include <string>

#include "md2ansi/md2ansi_block.hpp"
#include "md4c.h"

namespace see::md2ansi {

HBlock::HBlock() : BlockHandler(MD_BLOCK_H, "\033[1;32m") {}

auto HBlock::BlockStart(std::string& text, void* detail) -> void {
  auto h_detail = reinterpret_cast<MD_BLOCK_H_DETAIL*>(detail);
  std::string hashes(h_detail->level, '#');
  text += "\033[1;33m" + hashes + " " + GetStyle();
}

auto HBlock::BlockEnd(std::string& text, void* /* detail */) -> void {
  text += "\033[5m  \n";
}

}  // namespace see::md2ansi
