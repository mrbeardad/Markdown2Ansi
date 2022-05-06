#ifndef SEE_MD2ANSI_BLOCK_P_HPP_
#define SEE_MD2ANSI_BLOCK_P_HPP_

#include <string>

namespace see::md2ansi {

auto HandlePStart(std::string& text) -> void;

auto HandlePEnd(std::string& text) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_P_HPP_
