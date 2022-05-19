#include "md2ansi/table.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <vector>

#include "md2ansi/md2ansi_block.hpp"
#include "md4c.h"
#include "utils/string.hpp"
#include "utils/tty.hpp"

namespace see::md2ansi {

namespace {
std::vector<std::vector<std::string>> kTableData{};
std::vector<std::vector<int>> kTableDataDisplayWidth{};
const std::array kRowColors{"\033[38;5;240;48;2;94;175;220m", "\033[38;5;240;48;2;128;128;255m"};
const auto kColorClose = "\033[m";
const auto kTopLeft = "┌";
const auto kTopMid = "┬";
const auto kTopRight = "┐";
const auto kLeftMid = "├";
const auto kRightMid = "┤";
const auto kBottomLeft = "└";
const auto kBottomMid = "┴";
const auto kBottomRight = "┘";
const auto kHorLine = "─";
const auto kVertLine = "│";
const auto kPaddingSize = 1;
}  // namespace

TableBlock::TableBlock() : BlockHandler(MD_BLOCK_TABLE, ""), start_{0} {}

auto TableBlock::BlockStart(std::string& text, void* /* detail */) -> void {
  start_ = text.length();
}

auto TableBlock::BlockEnd(std::string& text, void* /* detail */) -> void {
  text.erase(start_);
  std::vector<int> col_maxwidth(kTableDataDisplayWidth.back().size());
  for (auto& row : kTableDataDisplayWidth) {
    for (size_t col{0}; col < row.size(); ++col) {
      col_maxwidth[col] = std::max(col_maxwidth[col], row[col]);
    }
  }

  // above table header
  text << kTopLeft;
  for (size_t col{0};;) {
    text << Repeat(kHorLine, col_maxwidth[col] + kPaddingSize * 2);
    if (++col >= col_maxwidth.size()) {
      text << kTopRight << '\n';
      break;
    }
    text << kTopMid;
  }

  // table header
  size_t row{0};
  for (size_t col{0}; col < col_maxwidth.size(); ++col) {
    text << kVertLine << std::string(kPaddingSize, ' ') << kTableData[row][col]
         << std::string(col_maxwidth[col] + kPaddingSize - kTableDataDisplayWidth[row][col], ' ');
  }
  text << kVertLine << '\n';
  ++row;

  // under table header
  text << kLeftMid;
  for (size_t col{0};;) {
    text << Repeat(kHorLine, col_maxwidth[col] + kPaddingSize * 2);
    if (++col >= col_maxwidth.size()) {
      text << kRightMid << '\n';
      break;
    }
    text << kVertLine;
  }

  // table data
  for (; row < kTableData.size(); ++row) {
    for (size_t col{0}; col < col_maxwidth.size(); ++col) {
      text << kVertLine << kRowColors.at(row % kRowColors.size()) << std::string(kPaddingSize, ' ')
           << kTableData[row][col] << kRowColors.at(row % kRowColors.size())
           << std::string(col_maxwidth[col] + kPaddingSize - kTableDataDisplayWidth[row][col], ' ');
    }
    text << kColorClose << kVertLine << '\n';
  }

  // table bottom
  text << kBottomLeft;
  for (size_t col{0};;) {
    text << Repeat(kHorLine, col_maxwidth[col] + kPaddingSize * 2);
    if (++col >= col_maxwidth.size()) {
      text << kBottomRight << '\n';
      break;
    }
    text << kBottomMid;
  }

  kTableData.clear();
  kTableDataDisplayWidth.clear();
}

TheadBlock::TheadBlock() : BlockHandler(MD_BLOCK_THEAD, "") {}

auto TheadBlock::BlockStart(std::string& /* text */, void* /* detail */) -> void {}

auto TheadBlock::BlockEnd(std::string& /* text */, void* /* detail */) -> void {}

TbodyBlock::TbodyBlock() : BlockHandler(MD_BLOCK_TBODY, "") {}

auto TbodyBlock::BlockStart(std::string& /* text */, void* /* detail */) -> void {}

auto TbodyBlock::BlockEnd(std::string& /* text */, void* /* detail */) -> void {}

TrBlock::TrBlock() : BlockHandler(MD_BLOCK_TR, "") {}

auto TrBlock::BlockStart(std::string& /* text */, void* /* detail */) -> void {
  kTableData.emplace_back();
  kTableDataDisplayWidth.emplace_back();
}

auto TrBlock::BlockEnd(std::string& /* text */, void* /* detail */) -> void {}

ThBlock::ThBlock() : BlockHandler(MD_BLOCK_TH, ""), start_{0} {}

auto ThBlock::BlockStart(std::string& text, void* /* detail */) -> void {
  start_ = text.length();
}

auto ThBlock::BlockEnd(std::string& text, void* /* detail */) -> void {
  auto cell_str = text.substr(start_);
  kTableData.back().emplace_back(cell_str);
  kTableDataDisplayWidth.back().emplace_back(utils::DisplayWidth(cell_str));
  text.erase(start_);
}

TdBlock::TdBlock() : BlockHandler(MD_BLOCK_TD, ""), start_{0} {}

auto TdBlock::BlockStart(std::string& text, void* /* detail */) -> void {
  start_ = text.length();
}

auto TdBlock::BlockEnd(std::string& text, void* /* detail */) -> void {
  auto cell_str = text.substr(start_);
  kTableData.back().emplace_back(cell_str);
  kTableDataDisplayWidth.back().emplace_back(utils::DisplayWidth(cell_str));
  text.erase(start_);
}

}  // namespace see::md2ansi
