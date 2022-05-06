#ifndef SEE_MD2ANSI_BLOCK_HTML_HPP_
#define SEE_MD2ANSI_BLOCK_HTML_HPP_

#include <string>
#include "md4c.h"

namespace see::md2ansi {

auto HandleHtmlStart(std::string& text) -> void;

auto HandleHtmlEnd(std::string& text) -> void;

auto HandleHtmlText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len)
    -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_HTML_HPP_
