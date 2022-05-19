#ifndef SEE_MD2ANSI_TABLE_HPP_
#define SEE_MD2ANSI_TABLE_HPP_

#include "md2ansi/md2ansi_block.hpp"

#include <cstddef>
#include <string>

namespace see::md2ansi {

class TableBlock : public BlockHandler {
 public:
  TableBlock();
  TableBlock(TableBlock&&) = default;
  TableBlock(const TableBlock&) = default;
  auto operator=(TableBlock&&) -> TableBlock& = default;
  auto operator=(const TableBlock&) -> TableBlock& = default;
  ~TableBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;

 private:
  size_t start_;
};

class TheadBlock : public BlockHandler {
 public:
  TheadBlock();
  TheadBlock(TheadBlock&&) = default;
  TheadBlock(const TheadBlock&) = default;
  auto operator=(TheadBlock&&) -> TheadBlock& = default;
  auto operator=(const TheadBlock&) -> TheadBlock& = default;
  ~TheadBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

class TbodyBlock : public BlockHandler {
 public:
  TbodyBlock();
  TbodyBlock(TbodyBlock&&) = default;
  TbodyBlock(const TbodyBlock&) = default;
  auto operator=(TbodyBlock&&) -> TbodyBlock& = default;
  auto operator=(const TbodyBlock&) -> TbodyBlock& = default;
  ~TbodyBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

class TrBlock : public BlockHandler {
 public:
  TrBlock();
  TrBlock(TrBlock&&) = default;
  TrBlock(const TrBlock&) = default;
  auto operator=(TrBlock&&) -> TrBlock& = default;
  auto operator=(const TrBlock&) -> TrBlock& = default;
  ~TrBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;
};

class ThBlock : public BlockHandler {
 public:
  ThBlock();
  ThBlock(ThBlock&&) = default;
  ThBlock(const ThBlock&) = default;
  auto operator=(ThBlock&&) -> ThBlock& = default;
  auto operator=(const ThBlock&) -> ThBlock& = default;
  ~ThBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;

 private:
  size_t start_;
};

class TdBlock : public BlockHandler {
 public:
  TdBlock();
  TdBlock(TdBlock&&) = default;
  TdBlock(const TdBlock&) = default;
  auto operator=(TdBlock&&) -> TdBlock& = default;
  auto operator=(const TdBlock&) -> TdBlock& = default;
  ~TdBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;

 private:
  size_t start_;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_TABLE_HPP_
