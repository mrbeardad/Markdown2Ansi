#ifndef SEE_MD2ANSI_A_HPP_
#define SEE_MD2ANSI_A_HPP_

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

class ASpan : public SpanHandler {
 public:
  ASpan();
  ASpan(ASpan&&) = default;
  ASpan(const ASpan&) = default;
  auto operator=(ASpan&&) -> ASpan& = default;
  auto operator=(const ASpan&) -> ASpan& = default;
  ~ASpan() override = default;

  void SpanStart(std::string& text, void* detail) override;
  void SpanEnd(std::string& text, void* detail) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_A_HPP_
