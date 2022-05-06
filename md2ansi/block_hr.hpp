#ifndef SEE_MD2ANSI_BLOCK_HR_HPP_
#define SEE_MD2ANSI_BLOCK_HR_HPP_

#include <string>

namespace see::md2ansi {

auto HandleHrStart(std::string& text) -> void;

auto HandleHrEnd(std::string& text) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_HR_HPP_
