#ifndef SEE_MD2ANSI_STRONG_HPP_
#define SEE_MD2ANSI_STRONG_HPP_

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

class StrongSpan : public SpanHandler {
 public:
  StrongSpan();
  StrongSpan(StrongSpan&&) = default;
  StrongSpan(const StrongSpan&) = default;
  auto operator=(StrongSpan&&) -> StrongSpan& = default;
  auto operator=(const StrongSpan&) -> StrongSpan& = default;
  ~StrongSpan() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_STRONG_HPP_
