#ifndef SEE_MD2ANSI_CODE_HPP_
#define SEE_MD2ANSI_CODE_HPP_

#include "md2ansi/md2ansi_block.hpp"
#include "md2ansi/md2ansi_span.hpp"
#include "md2ansi/md2ansi_text.hpp"

#include <string>

namespace see::md2ansi {

class CodeBlock : public BlockHandler {
 public:
  CodeBlock();
  CodeBlock(CodeBlock&&) = default;
  CodeBlock(const CodeBlock&) = default;
  auto operator=(CodeBlock&&) -> CodeBlock& = default;
  auto operator=(const CodeBlock&) -> CodeBlock& = default;
  ~CodeBlock() override = default;

  void BlockStart(std::string& text, void* detail) override;
  void BlockEnd(std::string& text, void* detail) override;

 private:
  std::string lang_;
  size_t code_begin_;
};

class CodeSpan : public SpanHandler {
 public:
  CodeSpan();
  CodeSpan(CodeSpan&&) = default;
  CodeSpan(const CodeSpan&) = default;
  auto operator=(CodeSpan&&) -> CodeSpan& = default;
  auto operator=(const CodeSpan&) -> CodeSpan& = default;
  ~CodeSpan() override = default;
};

class CodeText : public TextHandler {
 public:
  CodeText();
  CodeText(CodeText&&) = default;
  CodeText(const CodeText&) = default;
  auto operator=(CodeText&&) -> CodeText& = default;
  auto operator=(const CodeText&) -> CodeText& = default;
  ~CodeText() override = default;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_CODE_HPP_
