#ifndef SEE_MD2ANSI_P_HPP_
#define SEE_MD2ANSI_P_HPP_

#include "md2ansi/md2ansi_block.hpp"

#include <string>

namespace see::md2ansi {

class PBlock : public BlockHandler {
 public:
  PBlock();
  PBlock(PBlock&&) = default;
  PBlock(const PBlock&) = default;
  auto operator=(PBlock&&) -> PBlock& = default;
  auto operator=(const PBlock&) -> PBlock& = default;
  ~PBlock() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_P_HPP_
