#ifndef SEE_MD2ANSI_LATEX_HPP_
#define SEE_MD2ANSI_LATEX_HPP_

#include <array>
#include "md2ansi/md2ansi_span.hpp"
#include "md2ansi/md2ansi_text.hpp"

namespace see::md2ansi {

class LatexSpan : public SpanHandler {
 public:
  LatexSpan();
  LatexSpan(LatexSpan&&) = default;
  LatexSpan(const LatexSpan&) = default;
  auto operator=(LatexSpan&&) -> LatexSpan& = default;
  auto operator=(const LatexSpan&) -> LatexSpan& = default;
  ~LatexSpan() override = default;
};

class LatexDisplaySpan : public SpanHandler {
 public:
  LatexDisplaySpan();
  LatexDisplaySpan(LatexDisplaySpan&&) = default;
  LatexDisplaySpan(const LatexDisplaySpan&) = default;
  auto operator=(LatexDisplaySpan&&) -> LatexDisplaySpan& = default;
  auto operator=(const LatexDisplaySpan&) -> LatexDisplaySpan& = default;
  ~LatexDisplaySpan() override = default;

  void SpanStart(std::string& text, void* detail) override;
  void SpanEnd(std::string& text, void* detail) override;
};

class LatexText : public TextHandler {
 public:
  LatexText();
  LatexText(LatexText&&) = default;
  LatexText(const LatexText&) = default;
  auto operator=(LatexText&&) -> LatexText& = default;
  auto operator=(const LatexText&) -> LatexText& = default;
  ~LatexText() override = default;

  void HandleText(std::string& text, const MD_CHAR* raw, MD_SIZE len) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_LATEX_HPP_
