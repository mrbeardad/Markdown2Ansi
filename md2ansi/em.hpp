#ifndef SEE_MD2ANSI_EM_HPP_
#define SEE_MD2ANSI_EM_HPP_

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

class EmSpan : public SpanHandler {
 public:
  EmSpan();
  EmSpan(EmSpan&&) = default;
  EmSpan(const EmSpan&) = default;
  auto operator=(EmSpan&&) -> EmSpan& = default;
  auto operator=(const EmSpan&) -> EmSpan& = default;
  ~EmSpan() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_EM_HPP_
