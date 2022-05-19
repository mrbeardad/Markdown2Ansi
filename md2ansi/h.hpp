#ifndef SEE_MD2ANSI_H_HPP_
#define SEE_MD2ANSI_H_HPP_

#include "md2ansi/md2ansi_block.hpp"

#include <string>

namespace see::md2ansi {

class HBlock : public BlockHandler {
 public:
  HBlock();
  HBlock(HBlock&&) = default;
  HBlock(const HBlock&) = default;
  auto operator=(HBlock&&) -> HBlock& = default;
  auto operator=(const HBlock&) -> HBlock& = default;
  ~HBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_H_HPP_
