#ifndef SEE_MD2ANSI_SPAN_HPP_
#define SEE_MD2ANSI_SPAN_HPP_

#include <string>
#include <utility>

#include "md4c.h"

namespace see::md2ansi {

class SpanHandler {
 public:
  SpanHandler(MD_SPANTYPE type, std::string style) : type_{type}, style_{std::move(style)} {}

  SpanHandler(const SpanHandler&) = default;
  SpanHandler(SpanHandler&&) = default;
  auto operator=(SpanHandler&&) -> SpanHandler& = default;
  auto operator=(const SpanHandler&) -> SpanHandler& = default;
  virtual ~SpanHandler() = default;

  [[nodiscard]] auto GetType() const -> MD_SPANTYPE { return type_; }

  [[nodiscard]] auto GetStyle() const -> std::string { return style_; }

  void SetStyle(const std::string& new_style) { style_ = new_style; }

  virtual void SpanStart(std::string& text, void* /* detail */) { text += style_; }

  virtual void SpanEnd(std::string& /* text */, void* /* detail */) {}

 private:
  MD_SPANTYPE type_;
  std::string style_;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_SPAN_HPP_
