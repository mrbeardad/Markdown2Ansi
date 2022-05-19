#ifndef SEE_MD2ANSI_U_HPP_
#define SEE_MD2ANSI_U_HPP_

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

class USpan : public SpanHandler {
 public:
  using SpanHandler::SpanHandler;
  USpan();
  USpan(USpan&&) = default;
  USpan(const USpan&) = default;
  auto operator=(USpan&&) -> USpan& = default;
  auto operator=(const USpan&) -> USpan& = default;
  ~USpan() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_U_HPP_
