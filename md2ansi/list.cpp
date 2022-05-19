#include "md2ansi/list.hpp"

#include <array>
#include <string>
#include <vector>
#include "md2ansi/md2ansi_block.hpp"
#include "md4c.h"

namespace see::md2ansi {

namespace {

auto kListItemLevel = -1;
auto kOrderedListItemIndex = 0;
const std::array kUnorderedListItemMarks{"● ", "○ ", "■ ", "□ "};

}  // namespace

UlBlock::UlBlock() : BlockHandler{MD_BLOCK_UL, ""} {}

auto UlBlock::BlockStart(std::string& /* text */, void* /* detail */) -> void {
  ++kListItemLevel;
}

auto UlBlock::BlockEnd(std::string& /* text */, void* /* detail */) -> void {
  --kListItemLevel;
}

// BUG: indent and number
OlBlock::OlBlock() : BlockHandler{MD_BLOCK_OL, ""} {}

auto OlBlock::BlockStart(std::string& /* text */, void* /* detail */) -> void {
  ++kListItemLevel;
  kOrderedListItemIndex = 1;
}

auto OlBlock::BlockEnd(std::string& /* text */, void* /* detail */) -> void {
  --kListItemLevel;
  kOrderedListItemIndex = 0;
}

LiBlock::LiBlock() : BlockHandler{MD_BLOCK_LI, ""} {}

auto LiBlock::BlockStart(std::string& text, void* detail) -> void {
  auto li_detail = reinterpret_cast<MD_BLOCK_LI_DETAIL*>(detail);
  text += std::string(static_cast<size_t>(kListItemLevel) * 4, ' ');
  if (li_detail->is_task) {
    text += li_detail->task_mark == ' ' ? "🔳 " : "✅ ";
  } else if (kOrderedListItemIndex != 0) {
    text += "\033[1m" + std::to_string(kOrderedListItemIndex++) + ".\033[m ";
  } else {
    text += kUnorderedListItemMarks.at(kListItemLevel % kUnorderedListItemMarks.size());
  }
}

auto LiBlock::BlockEnd(std::string& /* text */, void* /* detail */) -> void {}

}  // namespace see::md2ansi
