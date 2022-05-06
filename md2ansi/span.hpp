#ifndef SEE_MD2ANSI_SPAN_HPP_
#define SEE_MD2ANSI_SPAN_HPP_

#include <string>

#include "md4c.h"

namespace see::md2ansi {

auto HandleEmStart(std::string& text) -> void;

auto HandleEmEnd(std::string& text) -> void;

auto HandleStrongStart(std::string& text) -> void;

auto HandleStrongEnd(std::string& text) -> void;

auto HandleAStart(std::string& text, void* detail) -> void;

auto HandleAEnd(std::string& text, void* detail) -> void;

auto HandleImgStart(std::string& text, void* detail) -> void;

auto HandleImgEnd(std::string& text, void* detail) -> void;

auto HandleCodeStart(std::string& text) -> void;

auto HandleCodeEnd(std::string& text) -> void;

auto HandleDelStart(std::string& text) -> void;

auto HandleDelEnd(std::string& text) -> void;

auto HandleUStart(std::string& text) -> void;

auto HandleUEnd(std::string& text) -> void;

auto HandleLatexStart(std::string& text) -> void;

auto HandleLatexEnd(std::string& text) -> void;

auto HandleLatexDisplayStart(std::string& text) -> void;

auto HandleLatexDisplayEnd(std::string& text) -> void;

auto HandleLatexText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len)
    -> void;

auto HandleEntityText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len)
    -> void;
}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_SPAN_HPP_
