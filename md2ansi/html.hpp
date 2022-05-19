#ifndef SEE_MD2ANSI_HTML_HPP_
#define SEE_MD2ANSI_HTML_HPP_

#include "md2ansi/md2ansi_text.hpp"

#include <string>

namespace see::md2ansi {

class HtmlText : public TextHandler {
 public:
  HtmlText();
  HtmlText(HtmlText&&) = default;
  HtmlText(const HtmlText&) = default;
  auto operator=(HtmlText&&) -> HtmlText& = default;
  auto operator=(const HtmlText&) -> HtmlText& = default;
  ~HtmlText() override = default;

  void HandleText(std::string& text, const MD_CHAR* raw, MD_SIZE len) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_HTML_HPP_
