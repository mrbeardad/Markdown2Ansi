#ifndef SEE_MD2ANSI_MD2ANSI_HPP_
#define SEE_MD2ANSI_MD2ANSI_HPP_

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "md2ansi/md2ansi_block.hpp"
#include "md2ansi/md2ansi_span.hpp"
#include "md2ansi/md2ansi_text.hpp"
#include "md4c.h"

namespace see::md2ansi {

class Md2Ansi {
  constexpr static auto kMdTypeMaxCount = 32;
  constexpr static auto kDefaultStyle = "\033[m";

  enum PosInBlock { kStart, kInner, kEnd };

 public:
  Md2Ansi();
  Md2Ansi(Md2Ansi&&) noexcept;
  Md2Ansi(const Md2Ansi&) = delete;
  auto operator=(Md2Ansi&&) noexcept -> Md2Ansi&;
  auto operator=(const Md2Ansi&) -> Md2Ansi& = delete;
  ~Md2Ansi();

  auto RegisterBlock(std::unique_ptr<BlockHandler> block_handler) -> bool;
  auto RegisterSpan(std::unique_ptr<SpanHandler> span_handler) -> bool;
  auto RegisterText(std::unique_ptr<TextHandler> text_handler) -> bool;

  auto EnterBlock(MD_BLOCKTYPE type, void* detail) -> int;
  auto LeaveBlock(MD_BLOCKTYPE type, void* detail) -> int;
  auto EnterSpan(MD_SPANTYPE type, void* detail) -> int;
  auto LeaveSpan(MD_SPANTYPE type, void* detail) -> int;
  auto TextHandle(MD_TEXTTYPE type, const MD_CHAR* raw, MD_SIZE len) -> int;

  auto operator()(const std::string& raw_text) -> std::string;

 private:
  std::string text_;
  std::array<std::unique_ptr<BlockHandler>, kMdTypeMaxCount> block_handlers_;
  std::array<std::unique_ptr<SpanHandler>, kMdTypeMaxCount> span_handlers_;
  std::array<std::unique_ptr<TextHandler>, kMdTypeMaxCount> text_handlers_;
  std::vector<std::string> context_;
  PosInBlock pos_;
};

auto MakeDefaultMd2Ansi() -> Md2Ansi;

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_MD2ANSI_HPP_
