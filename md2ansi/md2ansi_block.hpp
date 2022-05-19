#ifndef SEE_MD2ANSI_BLOCK_HPP_
#define SEE_MD2ANSI_BLOCK_HPP_

#include <string>
#include <utility>

#include "md4c.h"

namespace see::md2ansi {

// TODO: Add a interface GetOtherBlock()
class BlockHandler {
 public:
  explicit BlockHandler(MD_BLOCKTYPE type, std::string style)
      : type_{type}, style_{std::move(style)} {}

  BlockHandler(BlockHandler&&) = default;
  BlockHandler(const BlockHandler&) = default;
  auto operator=(BlockHandler&&) -> BlockHandler& = default;
  auto operator=(const BlockHandler&) -> BlockHandler& = default;
  virtual ~BlockHandler() = default;

  [[nodiscard]] auto GetType() const -> MD_BLOCKTYPE { return type_; }

  [[nodiscard]] auto GetStyle() const -> std::string { return style_; }

  void SetStyle(const std::string& new_style) { style_ = new_style; }

  virtual void BlockStart(std::string& text, void* /* detail */) { text += style_; }

  virtual void BlockEnd(std::string& /* text */, void* /* detail */) {}

 private:
  MD_BLOCKTYPE type_;
  std::string style_;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_BLOCK_HPP_
