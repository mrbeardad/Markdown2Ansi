#ifndef SEE_MD2ANSI_BLOCK_CODE_HPP_
#define SEE_MD2ANSI_BLOCK_CODE_HPP_

#include <string>

#include "md4c.h"

namespace see::md2ansi {

auto HandleCodeBlockStart(std::string& text, void* detail) -> void;

auto HandleCodeBlockEnd(std::string& text, void* detail) -> void;

auto HandleCodeBlockText(std::string& text,
                         const MD_CHAR* raw_text,
                         MD_SIZE len) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_CODE_HPP_
