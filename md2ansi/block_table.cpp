#include "md2ansi/block_table.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <vector>

#include "utils/string.hpp"
#include "utils/tty.hpp"

namespace see::md2ansi {

namespace {
// NOLINTNEXTLINE: reset each time
std::vector<std::vector<std::string>> kTableData{};
// NOLINTNEXTLINE: reset each time
std::vector<std::vector<int>> kTableDataDisplayWidth{};
const std::array kRowColors{"\033[38;5;240;48;2;94;175;220m",
                            "\033[38;5;240;48;2;128;128;255m"};
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

auto HandleTableStart(std::string& /* text */, void* /* detail */) -> void {}

auto HandleTableEnd(std::string& text, void* /* detail */) -> void {
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
        text << kVertLine << std::string(kPaddingSize, ' ')
             << kTableData[row][col]
             << std::string(col_maxwidth[col] + kPaddingSize -
                                kTableDataDisplayWidth[row][col],
                            ' ');
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
            text << kVertLine << kRowColors.at(row % kRowColors.size())
                 << std::string(kPaddingSize, ' ') << kTableData[row][col]
                 << kRowColors.at(row % kRowColors.size())
                 << std::string(col_maxwidth[col] + kPaddingSize -
                                    kTableDataDisplayWidth[row][col],
                                ' ');
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

auto HandleTheadStart(std::string& /* text */) -> void {}

auto HandleTheadEnd(std::string& /* text */) -> void {}

auto HandleTbodyStart(std::string& /* text */) -> void {}

auto HandleTbodyEnd(std::string& /* text */) -> void {}

auto HandleTrStart(std::string& /* text */) -> void {
    kTableData.emplace_back();
    kTableDataDisplayWidth.emplace_back();
}

auto HandleTrEnd(std::string& /* text */) -> void {}

auto HandleThStart(std::string& /* text */, void* /* detail */) -> void {}

auto HandleThEnd(std::string& text, void* /* detail */) -> void {
    auto start_pos = text.rfind('\n');
    auto cell_str = text.substr(start_pos + 1);
    kTableData.back().emplace_back(cell_str);
    kTableDataDisplayWidth.back().emplace_back(utils::DisplayWidth(cell_str));
    text.erase(start_pos);
}

auto HandleTdStart(std::string& /* text */, void* /* detail */) -> void {}

auto HandleTdEnd(std::string& text, void* /* detail */) -> void {
    auto start_pos = text.rfind('\n');
    auto cell_str = text.substr(start_pos + 1);
    kTableData.back().emplace_back(cell_str);
    kTableDataDisplayWidth.back().emplace_back(utils::DisplayWidth(cell_str));
    text.erase(start_pos);
}

}  // namespace see::md2ansi
