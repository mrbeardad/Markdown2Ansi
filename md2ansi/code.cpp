#include "md2ansi/code.hpp"

#include <sstream>
#include <string>
#include <vector>

#include "md4c.h"
#include "srchilite/sourcehighlight.h"

#include "md2ansi/md2ansi_block.hpp"
#include "md2ansi/md2ansi_span.hpp"
#include "md2ansi/md2ansi_text.hpp"
#include "utils/string.hpp"
#include "utils/tty.hpp"

namespace see::md2ansi {

namespace {

const auto kCodeLangColor = "\033[32m";
const auto kCodeEdgeColor = "\033[38;2;255;165;0m";
const auto kCodeColorClose = "\033[m";
const auto kTop = "─";
const auto kBottom = "─";
const auto kLeft = "│";
const auto kRight = "│";
const auto kTopLeft = "┌";
const auto kTopRight = "┐";
const auto kBottomLeft = "└";
const auto kBottomRight = "┘";
const auto kPaddingSize = 1;

}  // namespace

CodeBlock::CodeBlock() : BlockHandler(MD_BLOCK_CODE, ""), code_begin_{} {}

void CodeBlock::BlockStart(std::string& text, void* detail) {
  auto code_detail = reinterpret_cast<MD_BLOCK_CODE_DETAIL*>(detail);
  lang_.assign(code_detail->lang.text, code_detail->lang.size);
  code_begin_ = text.length();
}

auto CodeBlock::BlockEnd(std::string& text, void* /* detail */) -> void {
  std::stringstream sstrm{text.substr(code_begin_)};
  text.erase(code_begin_);

  std::vector<int> line_cols{};
  int max_col{};
  for (std::string oneline{}; std::getline(sstrm, oneline);) {
    auto cur_col = utils::DisplayWidth(oneline);
    line_cols.emplace_back(cur_col);
    max_col = std::max(max_col, cur_col);
  }
  sstrm.rdbuf()->pubseekpos(0);
  sstrm.clear();

  std::string first_line{};
  if (lang_.empty()) {
    for (int cnt{0}; cnt < max_col + kPaddingSize * 2; ++cnt) {
      first_line += kTop;
    }
  } else {
    auto total_count = max_col + kPaddingSize * 2 - (utils::DisplayWidth(lang_) + 2);
    auto left_count = total_count < 0 ? 0 : total_count / 2;
    for (int cnt{0}; cnt < left_count; ++cnt) {
      first_line += kTop;
    }
    first_line << " " << kCodeLangColor << lang_ << kCodeEdgeColor << " ";
    for (int cnt{0}; cnt < total_count - left_count; ++cnt) {
      first_line += kTop;
    }
  }
  text << kCodeEdgeColor << kTopLeft << first_line << kTopRight << kCodeColorClose << '\n';

  std::stringstream hilitStrm{};
  try {
    srchilite::SourceHighlight codeHilit{"esc.outlang"};
    codeHilit.highlight(sstrm, hilitStrm, lang_ + ".lang");
  } catch (std::exception&) {
    hilitStrm.str(sstrm.str());
  }

  size_t line_nr{0};
  for (std::string oneline{}; std::getline(hilitStrm, oneline);) {
    // oneline =
    //     std::regex_replace(oneline, "\033\\[01;30m", "\033[01;34m");
    text << kCodeEdgeColor << kLeft << kCodeColorClose << std::string(kPaddingSize, ' ') << oneline
         << std::string(max_col + kPaddingSize - line_cols[line_nr++], ' ') << kCodeEdgeColor
         << kRight << kCodeColorClose << '\n';
  }

  text << kCodeEdgeColor << kBottomLeft;
  for (int cnt{max_col + kPaddingSize * 2}; cnt > 0; --cnt) {
    text += kBottom;
  }
  text << kBottomRight << kCodeColorClose << '\n';
}

CodeSpan::CodeSpan() : SpanHandler(MD_SPAN_CODE, "\033[38;2;255;165;0m") {}

CodeText::CodeText() : TextHandler(MD_TEXT_CODE) {}

}  // namespace see::md2ansi
