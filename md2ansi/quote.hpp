#ifndef SEE_MD2ANSI_QUOTE_HPP_
#define SEE_MD2ANSI_QUOTE_HPP_

#include <string>
#include "md2ansi/md2ansi_block.hpp"

namespace see::md2ansi {

class QuoteBlock : public BlockHandler {
 public:
  QuoteBlock();
  QuoteBlock(QuoteBlock&&) = default;
  QuoteBlock(const QuoteBlock&) = default;
  auto operator=(QuoteBlock&&) -> QuoteBlock& = default;
  auto operator=(const QuoteBlock&) -> QuoteBlock& = default;
  ~QuoteBlock() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_QUOTE_HPP_
