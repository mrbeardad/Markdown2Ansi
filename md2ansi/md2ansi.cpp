#include "md2ansi/md2ansi.hpp"

#include <algorithm>
#include <memory>
#include <string>

#include "md2ansi/a.hpp"
#include "md2ansi/code.hpp"
#include "md2ansi/del.hpp"
#include "md2ansi/em.hpp"
#include "md2ansi/entity.hpp"
#include "md2ansi/h.hpp"
#include "md2ansi/hr.hpp"
#include "md2ansi/html.hpp"
#include "md2ansi/img.hpp"
#include "md2ansi/latex.hpp"
#include "md2ansi/list.hpp"
#include "md2ansi/md2ansi_span.hpp"
#include "md2ansi/normal.hpp"
#include "md2ansi/p.hpp"
#include "md2ansi/quote.hpp"
#include "md2ansi/strong.hpp"
#include "md2ansi/table.hpp"
#include "md2ansi/u.hpp"
#include "md4c.h"

namespace see::md2ansi {

Md2Ansi::Md2Ansi()
    : block_handlers_{},
      span_handlers_{},
      text_handlers_{},
      context_{kDefaultStyle},
      pos_{kStart} {}

Md2Ansi::Md2Ansi(Md2Ansi&&) noexcept = default;
auto Md2Ansi::operator=(Md2Ansi&&) noexcept -> Md2Ansi& = default;
Md2Ansi::~Md2Ansi() = default;

auto Md2Ansi::RegisterBlock(std::unique_ptr<BlockHandler> block_handler) -> bool {
  auto& target = block_handlers_.at(block_handler->GetType());
  if (target != nullptr) {
    return false;
  }
  target = std::move(block_handler);
  return true;
}

auto Md2Ansi::RegisterSpan(std::unique_ptr<SpanHandler> span_handler) -> bool {
  auto& target = span_handlers_.at(span_handler->GetType());
  if (target != nullptr) {
    return false;
  }
  target = std::move(span_handler);
  return true;
}

auto Md2Ansi::RegisterText(std::unique_ptr<TextHandler> text_handler) -> bool {
  auto& target = text_handlers_.at(text_handler->GetType());
  if (target != nullptr) {
    return false;
  }
  target = std::move(text_handler);
  return true;
}

auto Md2Ansi::EnterBlock(MD_BLOCKTYPE type, void* detail) -> int {
  auto& handler = block_handlers_.at(type);
  if (handler == nullptr) {
    return 0;
  }
  context_.emplace_back(handler->GetStyle());
  // The width of the block element is the same as the parent element,
  // add newline at block start when text already exists in current line
  // and is not block header.
  if (pos_ == kInner) {
    text_ += '\n';
  }
  pos_ = kStart;
  handler->BlockStart(text_, detail);
  return 0;
}

auto Md2Ansi::LeaveBlock(MD_BLOCKTYPE type, void* detail) -> int {
  auto& handler = block_handlers_.at(type);
  if (handler == nullptr) {
    return 0;
  }
  context_.pop_back();
  handler->BlockEnd(text_, detail);
  // reset style
  for (const auto& style : context_) {
    text_ += style;
  }
  // The child element is in the same box as its parent.
  if (pos_ != kEnd) {
    text_ += '\n';
  }
  pos_ = kEnd;
  return 0;
}

auto Md2Ansi::EnterSpan(MD_SPANTYPE type, void* detail) -> int {
  auto& handler = span_handlers_.at(type);
  if (handler == nullptr) {
    return 0;
  }
  context_.emplace_back(handler->GetStyle());
  handler->SpanStart(text_, detail);
  return 0;
}

auto Md2Ansi::LeaveSpan(MD_SPANTYPE type, void* detail) -> int {
  auto& handler = span_handlers_.at(type);
  if (handler == nullptr) {
    return 0;
  }
  context_.pop_back();
  handler->SpanEnd(text_, detail);
  // reset style
  for (const auto& style : context_) {
    text_ += style;
  }
  return 0;
}

auto Md2Ansi::TextHandle(MD_TEXTTYPE type, const MD_CHAR* raw, MD_SIZE len) -> int {
  auto& handler = text_handlers_.at(type);
  if (handler == nullptr) {
    return 0;
  }
  handler->HandleText(text_, raw, len);
  pos_ = kInner;
  return 0;
}

namespace {

// NOLINTNEXTLINE: Compatible with md4c
const auto kMd4cFlags = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES | MD_FLAG_STRIKETHROUGH
                        | MD_FLAG_TASKLISTS | MD_FLAG_LATEXMATHSPANS;

// NOLINTNEXTLINE: Compatible with md4c
Md2Ansi* kMd2Ansi{};

auto Md4cEnterBlock(MD_BLOCKTYPE type, void* detail, void* /*userdata*/) -> int {
  return kMd2Ansi->EnterBlock(type, detail);
}

auto Md4cLeaveBlock(MD_BLOCKTYPE type, void* detail, void* /*userdata*/) -> int {
  return kMd2Ansi->LeaveBlock(type, detail);
}

auto Md4cEnterSpan(MD_SPANTYPE type, void* detail, void* /*userdata*/) -> int {
  return kMd2Ansi->EnterSpan(type, detail);
}

auto Md4cLeaveSpan(MD_SPANTYPE type, void* detail, void* /*userdata*/) -> int {
  return kMd2Ansi->LeaveSpan(type, detail);
}

auto Md4cTextHandle(MD_TEXTTYPE type, const MD_CHAR* raw, MD_SIZE len, void* /*userdata*/) -> int {
  return kMd2Ansi->TextHandle(type, raw, len);
}

};  // namespace

auto Md2Ansi::operator()(const std::string& raw_text) -> std::string {
  kMd2Ansi = this;

  MD_PARSER parser{};
  parser.flags = kMd4cFlags;
  parser.enter_block = Md4cEnterBlock;
  parser.leave_block = Md4cLeaveBlock;
  parser.enter_span = Md4cEnterSpan;
  parser.leave_span = Md4cLeaveSpan;
  parser.text = Md4cTextHandle;

  md_parse(raw_text.c_str(), raw_text.length(), &parser, nullptr);

  auto ret = text_;
  text_.clear();
  context_.assign({kDefaultStyle});
  pos_ = kStart;
  return ret;
}

auto MakeDefaultMd2Ansi() -> Md2Ansi {
  Md2Ansi md2ansi{};

  md2ansi.RegisterBlock(std::make_unique<CodeBlock>());
  md2ansi.RegisterBlock(std::make_unique<HBlock>());
  md2ansi.RegisterBlock(std::make_unique<HrBlock>());
  md2ansi.RegisterBlock(std::make_unique<UlBlock>());
  md2ansi.RegisterBlock(std::make_unique<OlBlock>());
  md2ansi.RegisterBlock(std::make_unique<LiBlock>());
  md2ansi.RegisterBlock(std::make_unique<PBlock>());
  md2ansi.RegisterBlock(std::make_unique<QuoteBlock>());
  md2ansi.RegisterBlock(std::make_unique<TableBlock>());
  md2ansi.RegisterBlock(std::make_unique<TheadBlock>());
  md2ansi.RegisterBlock(std::make_unique<TbodyBlock>());
  md2ansi.RegisterBlock(std::make_unique<TrBlock>());
  md2ansi.RegisterBlock(std::make_unique<ThBlock>());
  md2ansi.RegisterBlock(std::make_unique<TdBlock>());

  md2ansi.RegisterSpan(std::make_unique<ASpan>());
  md2ansi.RegisterSpan(std::make_unique<CodeSpan>());
  md2ansi.RegisterSpan(std::make_unique<DelSpan>());
  md2ansi.RegisterSpan(std::make_unique<EmSpan>());
  md2ansi.RegisterSpan(std::make_unique<ImgSpan>());
  md2ansi.RegisterSpan(std::make_unique<LatexSpan>());
  md2ansi.RegisterSpan(std::make_unique<LatexDisplaySpan>());
  md2ansi.RegisterSpan(std::make_unique<StrongSpan>());
  md2ansi.RegisterSpan(std::make_unique<USpan>());

  md2ansi.RegisterText(std::make_unique<NormalText>());
  md2ansi.RegisterText(std::make_unique<CodeText>());
  md2ansi.RegisterText(std::make_unique<EntityText>());
  md2ansi.RegisterText(std::make_unique<HtmlText>());
  md2ansi.RegisterText(std::make_unique<LatexText>());

  return md2ansi;
}

}  // namespace see::md2ansi
