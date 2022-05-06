#include "md2ansi/block_h.hpp"
#include <string>
#include "md4c.h"

namespace see::md2ansi {

auto HandleHStart(std::string& text, void* detail) -> void {
    auto* h_detail = reinterpret_cast<MD_BLOCK_H_DETAIL*>(detail);
    std::string hashes(h_detail->level, '#');
    text += "\033[1;33m" + hashes + "\033[1;32m ";
}

auto HandleHEnd(std::string& text, void* /* detail */) -> void {
    text += "\033[5m ⁋\033[m\n";
}

}  // namespace see::md2ansi
