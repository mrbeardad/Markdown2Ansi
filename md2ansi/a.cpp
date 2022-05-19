#include "md2ansi/a.hpp"

#include <string>

#include "md2ansi/md2ansi_span.hpp"
#include "md4c.h"

namespace see::md2ansi {

ASpan::ASpan() : SpanHandler(MD_SPAN_A, "\033[38;2;0;152;245;4m") {}

void ASpan::SpanStart(std::string& text, void* /* detail */) {
  text += " \033[37m[" + GetStyle();
}

void ASpan::SpanEnd(std::string& text, void* detail) {
  auto a_detail = reinterpret_cast<MD_SPAN_A_DETAIL*>(detail);

  text += "\033[24;37m](" + GetStyle();
  text.append(a_detail->href.text, a_detail->href.size);
  text += "\033[24m";
  if (a_detail->title.size != 0) {
    text += " \033[2m\"";
    text.append(a_detail->title.text, a_detail->title.size);
    text += "\"\033[22m";
  }
  text += "\033[37m) ";
}

}  // namespace see::md2ansi
