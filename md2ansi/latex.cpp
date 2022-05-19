#include "md2ansi/latex.hpp"

#include <algorithm>
#include <cctype>
#include <optional>
#include <string>
#include <string_view>

#include "md4c.h"
#include "third_party/eternal/include/mapbox/eternal.hpp"

namespace see::md2ansi {

LatexSpan::LatexSpan() : SpanHandler{MD_SPAN_LATEXMATH, "\033[38;2;255;165;0m"} {}

LatexDisplaySpan::LatexDisplaySpan()
    : SpanHandler{MD_SPAN_LATEXMATH_DISPLAY, "\033[38;2;255;165;0m"} {}

void LatexDisplaySpan::SpanStart(std::string& text, void* /* detail */) {
  text += '\n';
}

void LatexDisplaySpan::SpanEnd(std::string& text, void* /* detail */) {
  text += '\n';
}

namespace {

const auto kSubs = mapbox::eternal::map<char, const char*>({
    {'0', "₀"}, {'1', "₁"}, {'2', "₂"}, {'3', "₃"},  {'4', "₄"}, {'5', "₅"}, {'6', "₆"}, {'7', "₇"},
    {'8', "₈"}, {'9', "₉"}, {'a', "ₐ"}, {'e', "ₑ"},  {'h', "ₕ"}, {'i', "ᵢ"}, {'j', "ⱼ"}, {'k', "ₖ"},
    {'l', "ₗ"}, {'m', "ₘ"}, {'n', "ₙ"}, {'o', "ₒ"},  {'p', "ₚ"}, {'r', "ᵣ"}, {'s', "ₛ"}, {'t', "ₜ"},
    {'u', "ᵤ"}, {'v', "ᵥ"}, {'x', "ₓ"}, {',', "︐"}, {'+', "₊"}, {'-', "₋"}, {'/', "ˏ"}, {'(', "₍"},
    {')', "₎"}, {'.', "‸"}, {'r', "ᵣ"}, {'v', "ᵥ"},  {'x', "ₓ"},
});

const auto kSupers = mapbox::eternal::map<char, const char*>({
    {'0', "⁰"},  {'1', "¹"}, {'2', "²"}, {'3', "³"}, {'4', "⁴"}, {'5', "⁵"},  {'6', "⁶"},
    {'7', "⁷"},  {'8', "⁸"}, {'9', "⁹"}, {'a', "ᵃ"}, {'b', "ᵇ"}, {'c', "ᶜ"},  {'d', "ᵈ"},
    {'e', "ᵉ"},  {'f', "ᶠ"}, {'g', "ᵍ"}, {'h', "ʰ"}, {'i', "ⁱ"}, {'j', "ʲ"},  {'k', "ᵏ"},
    {'l', "ˡ"},  {'m', "ᵐ"}, {'n', "ⁿ"}, {'o', "ᵒ"}, {'p', "ᵖ"}, {'r', "ʳ"},  {'s', "ˢ"},
    {'t', "ᵗ"},  {'u', "ᵘ"}, {'v', "ᵛ"}, {'w', "ʷ"}, {'x', "ˣ"}, {'y', "ʸ"},  {'z', "ᶻ"},
    {'A', "ᴬ"},  {'B', "ᴮ"}, {'D', "ᴰ"}, {'E', "ᴱ"}, {'G', "ᴳ"}, {'H', "ᴴ"},  {'I', "ᴵ"},
    {'J', "ᴶ"},  {'K', "ᴷ"}, {'L', "ᴸ"}, {'M', "ᴹ"}, {'N', "ᴺ"}, {'O', "ᴼ"},  {'P', "ᴾ"},
    {'R', "ᴿ"},  {'T', "ᵀ"}, {'U', "ᵁ"}, {'V', "ⱽ"}, {'W', "ᵂ"}, {',', "︐"}, {':', "︓"},
    {';', "︔"}, {'+', "⁺"}, {'-', "⁻"}, {'<', "˂"}, {'>', "˃"}, {'/', "ˊ"},  {'(', "⁽"},
    {')', "⁾"},  {'.', "˙"}, {'=', "˭"},
});

std::optional<std::string> ConvertTex(const std::string& tex, bool is_super) {
  std::string ret{};
  if (is_super) {
    for (auto chr : tex) {
      auto pos = kSupers.find(chr);
      if (pos == kSupers.end()) {
        return {};
      }
      ret += pos->second;
    }
  } else {
    for (auto chr : tex) {
      auto pos = kSubs.find(chr);
      if (pos == kSubs.end()) {
        return {};
      }
      ret += pos->second;
    }
  }
  return {ret};
}

}  // namespace

LatexText::LatexText() : TextHandler{MD_TEXT_LATEXMATH} {}

void LatexText::HandleText(std::string& text, const MD_CHAR* raw, MD_SIZE len) {
  size_t idx{0};
  auto get_escape_sequence = [&idx, raw, len]() {
    std::string escape{"\\"};
    for (; ++idx < len && std::isalpha(raw[idx]);) {
      escape += raw[len];
    }
    return escape;
  };

  for (; idx < len;) {
    if (raw[idx] == '\033') {
      text += get_escape_sequence();
    } else if (raw[idx] == '_' || raw[idx] == '^') {
      bool is_super{raw[idx] == '^'};
      if (++idx >= len) {
        break;
      }
      std::string super_sub{};
      bool has_escape_in_brackets{false};
      bool in_bracket{false};
      if (raw[idx] == '{') {
        in_bracket = true;
        for (; ++idx < len && (raw[idx] != '}' || raw[idx - 1] == '\\');) {
          has_escape_in_brackets = raw[idx] == '\\' ? true : has_escape_in_brackets;
          super_sub += raw[idx];
        }
        ++idx;
      } else {
        super_sub += raw[idx++];
      }
      // no escape in super_sub
      std::optional<std::string> cvted_super_sub{};
      if (!has_escape_in_brackets && (cvted_super_sub = ConvertTex(super_sub, is_super))) {
        text += *cvted_super_sub;
      } else {
        text += is_super ? '^' : '_';
        if (in_bracket) {
          text += "{" + super_sub + '}';
        } else {
          text += super_sub;
        }
      }
    } else {
      text += raw[idx++];
    }
  }
}

}  // namespace see::md2ansi
