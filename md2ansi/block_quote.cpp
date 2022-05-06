#include "md2ansi/block_quote.hpp"

namespace see::md2ansi {

auto HandleQuoteStart(std::string& text) -> void {
    text += "\033[2m";
}

auto HandleQuoteEnd(std::string& text) -> void {
    text += "\033[m";
}

}  // namespace see::md2ansi
