#ifndef SEE_MD2ANSI_LIST_HPP_
#define SEE_MD2ANSI_LIST_HPP_

#include "md2ansi/md2ansi_block.hpp"

#include <string>

namespace see::md2ansi {

class UlBlock : public BlockHandler {
 public:
  UlBlock();
  UlBlock(UlBlock&&) = default;
  UlBlock(const UlBlock&) = default;
  auto operator=(UlBlock&&) -> UlBlock& = default;
  auto operator=(const UlBlock&) -> UlBlock& = default;
  ~UlBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

class OlBlock : public BlockHandler {
 public:
  OlBlock();
  OlBlock(OlBlock&&) = default;
  OlBlock(const OlBlock&) = default;
  auto operator=(OlBlock&&) -> OlBlock& = default;
  auto operator=(const OlBlock&) -> OlBlock& = default;
  ~OlBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

class LiBlock : public BlockHandler {
 public:
  LiBlock();
  LiBlock(LiBlock&&) = default;
  LiBlock(const LiBlock&) = default;
  auto operator=(LiBlock&&) -> LiBlock& = default;
  auto operator=(const LiBlock&) -> LiBlock& = default;
  ~LiBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_LIST_HPP_
