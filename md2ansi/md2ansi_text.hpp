#ifndef SEE_MD2ANSI_TEXT_HPP_
#define SEE_MD2ANSI_TEXT_HPP_

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "md4c.h"

namespace see::md2ansi {

class TextHandler {
 public:
  explicit TextHandler(MD_TEXTTYPE type) : type_{type} {}

  TextHandler(TextHandler&&) = default;
  TextHandler(const TextHandler&) = default;
  auto operator=(TextHandler&&) -> TextHandler& = default;
  auto operator=(const TextHandler&) -> TextHandler& = default;
  virtual ~TextHandler() = default;

  [[nodiscard]] auto GetType() const -> MD_TEXTTYPE { return type_; }

  virtual void HandleText(std::string& text, const MD_CHAR* raw, MD_SIZE len) {
    text.append(raw, len);
  }

 private:
  MD_TEXTTYPE type_;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_TEXT_HPP_
