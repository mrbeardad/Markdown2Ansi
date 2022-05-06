#ifndef SEE_MD2ANSI_BLOCK_LIST_HPP_
#define SEE_MD2ANSI_BLOCK_LIST_HPP_

#include <string>

namespace see::md2ansi {

auto HandleUlStart(std::string& text, void* detail) -> void;

auto HandleUlEnd(std::string& text, void* detail) -> void;

auto HandleOlStart(std::string& text, void* detail) -> void;

auto HandleOlEnd(std::string& text, void* detail) -> void;

auto HandleLiStart(std::string& text, void* detail) -> void;

auto HandleLiEnd(std::string& text, void* detail) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_LIST_HPP_
