#ifndef SEE_MD2ANSI_DEL_HPP_
#define SEE_MD2ANSI_DEL_HPP_

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

class DelSpan : public SpanHandler {
 public:
  DelSpan();
  DelSpan(DelSpan&&) = default;
  DelSpan(const DelSpan&) = default;
  auto operator=(DelSpan&&) -> DelSpan& = default;
  auto operator=(const DelSpan&) -> DelSpan& = default;
  ~DelSpan() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_DEL_HPP_
