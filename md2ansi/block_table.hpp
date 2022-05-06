#ifndef SEE_MD2ANSI_BLOCK_TABLE_HPP_
#define SEE_MD2ANSI_BLOCK_TABLE_HPP_

#include <string>

namespace see::md2ansi {

auto HandleTableStart(std::string& text, void* detail) -> void;

auto HandleTableEnd(std::string& text, void* detail) -> void;

auto HandleTheadStart(std::string& text) -> void;

auto HandleTheadEnd(std::string& text) -> void;

auto HandleTbodyStart(std::string& text) -> void;

auto HandleTbodyEnd(std::string& text) -> void;

auto HandleTrStart(std::string& text) -> void;

auto HandleTrEnd(std::string& text) -> void;

auto HandleThStart(std::string& text, void* detail) -> void;

auto HandleThEnd(std::string& text, void* detail) -> void;

auto HandleTdStart(std::string& text, void* detail) -> void;

auto HandleTdEnd(std::string& text, void* detail) -> void;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_TABLE_HPP_
