#ifndef SEE_MD2ANSI_BLOCK_QUOTE_HPP_
#define SEE_MD2ANSI_BLOCK_QUOTE_HPP_

#include <string>

namespace see::md2ansi {

auto HandleQuoteStart(std::string& text) -> void;

auto HandleQuoteEnd(std::string& text) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_QUOTE_HPP_
