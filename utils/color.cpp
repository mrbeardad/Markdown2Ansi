#include "color.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "boost/locale/encoding_utf.hpp"

#include "utils/string.hpp"

namespace see::utils {

namespace {

const uint8_t kColorCodeGap = 0x5;
const uint8_t kColorCodeMax = 0xff;
const auto kDefaultColor = U"\033[m";

void NextColor(std::array<uint8_t, 3>& rgb) {
  auto rgb_size = rgb.size();
  auto ff_idx = rgb_size;
  // Locate 0xff color code
  for (; --ff_idx < rgb_size && rgb.at(ff_idx) != kColorCodeMax;) {
  }
  if (ff_idx == rgb_size - 1 && rgb[0] == kColorCodeMax) {
    ff_idx = 0;
  }
  // Locate color code that need change: increase previous color code or
  // decrease next one
  if (auto prev_idx = (ff_idx + rgb_size - 1) % rgb_size; rgb.at(prev_idx) != 0) {
    rgb.at(prev_idx) -= kColorCodeGap;
  } else {
    rgb.at((ff_idx + 1) % rgb_size) += kColorCodeGap;
  }
}

auto MakeAnsiColorCode(const std::array<uint8_t, 3>& rgb, bool isBg = false)
    -> std::basic_string<char32_t> {
  std::stringstream color_code{};
  if (isBg) {
    color_code << "\033[48;2;";
  } else {
    color_code << "\033[38;2;";
  }
  color_code << static_cast<int>(rgb[0]) << ';' << static_cast<int>(rgb[1]) << ';'
             << static_cast<int>(rgb[2]) << 'm';
  return boost::locale::conv::utf_to_utf<char32_t>(color_code.str());
}

}  // namespace

auto RainbowText(const std::string& text) -> std::string {
  std::array<uint8_t, 3> rgb{kColorCodeMax, 0, 0};

  auto unicode_text = boost::locale::conv::utf_to_utf<char32_t>(text);
  std::basic_string<char32_t> rainbow_wtext{};
  for (auto cur_wchar : unicode_text) {
    std::array<uint8_t, 3> bg_rgb{static_cast<uint8_t>(~rgb[0]), static_cast<uint8_t>(~rgb[1]),
                                  static_cast<uint8_t>(~rgb[2])};

    rainbow_wtext << MakeAnsiColorCode(rgb) << MakeAnsiColorCode(bg_rgb, true) << cur_wchar;
    NextColor(rgb);
  }
  rainbow_wtext << kDefaultColor;
  return boost::locale::conv::utf_to_utf<char>(rainbow_wtext);
}

}  // namespace see::utils
