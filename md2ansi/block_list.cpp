#include "md2ansi/block_list.hpp"
#include <array>
#include <string>
#include <vector>
#include "md4c.h"

namespace see::md2ansi {

namespace {

// NOLINTNEXTLINE: reset each time
auto kListItemLevel = -1;
// NOLINTNEXTLINE: reset each time
auto kOrderedListItemIndex = 0;
const std::array kUnorderedListItemMarks{"● \x01", "○ \x01", "■ \x01",
                                         "□ \x01"};

}  // namespace

auto HandleUlStart(std::string& /* text */, void* /* detail */) -> void {
    ++kListItemLevel;
}

auto HandleUlEnd(std::string& /* text */, void* /* detail */) -> void {
    --kListItemLevel;
}

auto HandleOlStart(std::string& /* text */, void* /* detail */) -> void {
    ++kListItemLevel;
    kOrderedListItemIndex = 1;
}

auto HandleOlEnd(std::string& /* text */, void* /* detail */) -> void {
    --kListItemLevel;
    kOrderedListItemIndex = 0;
}

auto HandleLiStart(std::string& text, void* detail) -> void {
    auto* li_detail = reinterpret_cast<MD_BLOCK_LI_DETAIL*>(detail);
    text += std::string(static_cast<size_t>(kListItemLevel) * 4, ' ');
    if (li_detail->is_task) {
        text += li_detail->task_mark == ' ' ? "🔳 \x01" : "✅ \x01";
    } else if (kOrderedListItemIndex != 0) {
        text += "\033[1m" + std::to_string(kOrderedListItemIndex++) +
                ".\033[m \x01";
    } else {
        text += kUnorderedListItemMarks.at(kListItemLevel %
                                           kUnorderedListItemMarks.size());
    }
}

auto HandleLiEnd(std::string& /* text */, void* /* detail */) -> void {}

}  // namespace see::md2ansi
