#ifndef SEE_MD2ANSI_BLOCK_H_HPP_
#define SEE_MD2ANSI_BLOCK_H_HPP_

#include <string>

namespace see::md2ansi {

auto HandleHStart(std::string& text, void* detail) -> void;

auto HandleHEnd(std::string& text, void* detail) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_H_HPP_
