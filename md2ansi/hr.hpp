#ifndef SEE_MD2ANSI_HR_HPP_
#define SEE_MD2ANSI_HR_HPP_

#include "md2ansi/md2ansi_block.hpp"

#include <string>

namespace see::md2ansi {

class HrBlock : public BlockHandler {
 public:
  HrBlock();
  HrBlock(HrBlock&&) = default;
  HrBlock(const HrBlock&) = default;
  auto operator=(HrBlock&&) -> HrBlock& = default;
  auto operator=(const HrBlock&) -> HrBlock& = default;
  ~HrBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_HR_HPP_
