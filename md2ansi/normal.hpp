#ifndef SEE_MD2ANSI_NORMAL_HPP_
#define SEE_MD2ANSI_NORMAL_HPP_

#include "md2ansi/md2ansi_text.hpp"

namespace see::md2ansi {

class NormalText : public TextHandler {
 public:
  NormalText();
  NormalText(NormalText&&) = default;
  NormalText(const NormalText&) = default;
  auto operator=(NormalText&&) -> NormalText& = default;
  auto operator=(const NormalText&) -> NormalText& = default;
  ~NormalText() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_NORMAL_HPP_
