#include "color.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <string>

#include "boost/locale/encoding_utf.hpp"

#include "utils/string.hpp"
#include "utils/tty.hpp"

namespace see::utils {

namespace {

const auto kColorCodeGap = 0x5;
const auto kColorCodeMax = 0xff;
const auto kDefaultColor = L"\033[m";

void NextColor(std::array<unsigned char, 3>& rgb) {
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
    if (auto prev_idx = (ff_idx + rgb_size - 1) % rgb_size;
        rgb.at(prev_idx) != 0) {
        rgb.at(prev_idx) -= kColorCodeGap;
    } else {
        rgb.at((ff_idx + 1) % rgb_size) += kColorCodeGap;
    }
}

auto MakeAnsiColorCode(std::array<unsigned char, 3>& rgb, bool isBg = false)
    -> std::wstring {
    std::wstringstream color_code{};
    if (isBg) {
        color_code << L"\033[48;2;";
    } else {
        color_code << L"\033[38;2;";
    }
    color_code << rgb[0] << L';' << rgb[1] << L';' << rgb[2] << L'm';
    return color_code.str();
}

}  // namespace

auto RainbowText(const std::string& text) -> std::string {
    std::array<unsigned char, 3> rgb{kColorCodeMax, 0, 0};

    auto wtext = boost::locale::conv::utf_to_utf<wchar_t>(text);
    std::wstring rainbow_wtext{};
    for (auto cur_wchar : wtext) {
        std::array<unsigned char, 3> bg_rgb{
            static_cast<unsigned char>(~rgb[0]),
            static_cast<unsigned char>(~rgb[1]),
            static_cast<unsigned char>(~rgb[2])};
        rainbow_wtext << MakeAnsiColorCode(rgb)
                      << MakeAnsiColorCode(bg_rgb, true) << cur_wchar;
        NextColor(rgb);
    }
    rainbow_wtext << kDefaultColor;
    return boost::locale::conv::utf_to_utf<char>(rainbow_wtext);
}

}  // namespace see::utils
