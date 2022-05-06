#include "md2ansi/span.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <string>

namespace see::md2ansi {

auto HandleEmStart(std::string& text) -> void {
    text += "\033[3m";
}

auto HandleEmEnd(std::string& text) -> void {
    text += "\033[m";
}

auto HandleStrongStart(std::string& text) -> void {
    text += "\033[1;37m";
}

auto HandleStrongEnd(std::string& text) -> void {
    text += "\033[m";
}

auto HandleAStart(std::string& text, void* /* detail */) -> void {
    text += " \033[m[\033[4;34m";
}

auto HandleAEnd(std::string& text, void* detail) -> void {
    auto* a_detail = reinterpret_cast<MD_SPAN_A_DETAIL*>(detail);
    text += "\033[m](\033[4;34m";
    text.append(a_detail->href.text, a_detail->href.size);
    text += "\033[m) ";
}

auto HandleImgStart(std::string& text, void* /* detail */) -> void {
    text += " ![\033[4;34";
}

auto HandleImgEnd(std::string& text, void* detail) -> void {
    auto* img_detail = reinterpret_cast<MD_SPAN_IMG_DETAIL*>(detail);
    text += "\033[m](\033[4;34m";
    text.append(img_detail->src.text, img_detail->src.size);
    text += "\033[m) ";
}

auto HandleCodeStart(std::string& text) -> void {
    text += "\033[30;46m ";
}

auto HandleCodeEnd(std::string& text) -> void {
    text += " \033[m";
}

auto HandleDelStart(std::string& text) -> void {
    text += "\033[9m";
}

auto HandleDelEnd(std::string& text) -> void {
    text += "\033[m";
}

auto HandleUStart(std::string& text) -> void {
    text += "\033[4m";
}

auto HandleUEnd(std::string& text) -> void {
    text += "\033[m";
}

auto HandleLatexStart(std::string& text) -> void {
    text += "\033[30;46m ";
}

auto HandleLatexEnd(std::string& text) -> void {
    text += " \033[m";
}

auto HandleLatexDisplayStart(std::string& text) -> void {
    if (!text.empty()) {
        if (text.back() == '\x01') {
            text.pop_back();
        } else if (text.back() != '\n') {
            text += '\n';
        }
    }
    text += "\033[30;46m ";
}

auto HandleLatexDisplayEnd(std::string& text) -> void {
    text += " \033[m\n";
}

namespace {

const auto kAsciiMaxCount = 128;

class SubArray {
   public:
    SubArray() {
        sub_['0'] = "₀";
        sub_['1'] = "₁";
        sub_['2'] = "₂";
        sub_['3'] = "₃";
        sub_['4'] = "₄";
        sub_['5'] = "₅";
        sub_['6'] = "₆";
        sub_['7'] = "₇";
        sub_['8'] = "₈";
        sub_['9'] = "₉";
        sub_['a'] = "ₐ";
        sub_['e'] = "ₑ";
        sub_['h'] = "ₕ";
        sub_['i'] = "ᵢ";
        sub_['j'] = "ⱼ";
        sub_['k'] = "ₖ";
        sub_['l'] = "ₗ";
        sub_['m'] = "ₘ";
        sub_['n'] = "ₙ";
        sub_['o'] = "ₒ";
        sub_['p'] = "ₚ";
        sub_['r'] = "ᵣ";
        sub_['s'] = "ₛ";
        sub_['t'] = "ₜ";
        sub_['u'] = "ᵤ";
        sub_['v'] = "ᵥ";
        sub_['x'] = "ₓ";
        sub_[','] = "︐";
        sub_['+'] = "₊";
        sub_['-'] = "₋";
        sub_['/'] = "ˏ";
        sub_['('] = "₍";
        sub_[')'] = "₎";
        sub_['.'] = "‸";
        sub_['r'] = "ᵣ";
        sub_['v'] = "ᵥ";
        sub_['x'] = "ₓ";
    }

    auto operator[](char chr) const -> const char* { return sub_.at(chr); }

   private:
    std::array<const char*, kAsciiMaxCount> sub_{};
};
const SubArray kSub{};

auto ConvertTexSub(const std::string& input) -> std::string {
    std::string ret{};
    for (auto chr : input) {
        ret += kSub[chr];
    }
    return ret;
}

auto CanBeSub(char input) -> bool {
    return kSub[input] != nullptr;
}

struct SuperArray {
   public:
    SuperArray() {
        super_['0'] = "⁰";
        super_['1'] = "¹";
        super_['2'] = "²";
        super_['3'] = "³";
        super_['4'] = "⁴";
        super_['5'] = "⁵";
        super_['6'] = "⁶";
        super_['7'] = "⁷";
        super_['8'] = "⁸";
        super_['9'] = "⁹";
        super_['a'] = "ᵃ";
        super_['b'] = "ᵇ";
        super_['c'] = "ᶜ";
        super_['d'] = "ᵈ";
        super_['e'] = "ᵉ";
        super_['f'] = "ᶠ";
        super_['g'] = "ᵍ";
        super_['h'] = "ʰ";
        super_['i'] = "ⁱ";
        super_['j'] = "ʲ";
        super_['k'] = "ᵏ";
        super_['l'] = "ˡ";
        super_['m'] = "ᵐ";
        super_['n'] = "ⁿ";
        super_['o'] = "ᵒ";
        super_['p'] = "ᵖ";
        super_['r'] = "ʳ";
        super_['s'] = "ˢ";
        super_['t'] = "ᵗ";
        super_['u'] = "ᵘ";
        super_['v'] = "ᵛ";
        super_['w'] = "ʷ";
        super_['x'] = "ˣ";
        super_['y'] = "ʸ";
        super_['z'] = "ᶻ";
        super_['A'] = "ᴬ";
        super_['B'] = "ᴮ";
        super_['D'] = "ᴰ";
        super_['E'] = "ᴱ";
        super_['G'] = "ᴳ";
        super_['H'] = "ᴴ";
        super_['I'] = "ᴵ";
        super_['J'] = "ᴶ";
        super_['K'] = "ᴷ";
        super_['L'] = "ᴸ";
        super_['M'] = "ᴹ";
        super_['N'] = "ᴺ";
        super_['O'] = "ᴼ";
        super_['P'] = "ᴾ";
        super_['R'] = "ᴿ";
        super_['T'] = "ᵀ";
        super_['U'] = "ᵁ";
        super_['V'] = "ⱽ";
        super_['W'] = "ᵂ";
        super_[','] = "︐";
        super_[':'] = "︓";
        super_[';'] = "︔";
        super_['+'] = "⁺";
        super_['-'] = "⁻";
        super_['<'] = "˂";
        super_['>'] = "˃";
        super_['/'] = "ˊ";
        super_['('] = "⁽";
        super_[')'] = "⁾";
        super_['.'] = "˙";
        super_['='] = "˭";
    }

    auto operator[](char chr) const -> const char* { return super_.at(chr); }

   private:
    std::array<const char*, kAsciiMaxCount> super_;
};
const SuperArray kSuper{};

auto ConvertTexSuper(const std::string& input) -> std::string {
    std::string ret{};
    for (auto chr : input) {
        ret += kSuper[chr];
    }
    return ret;
}

auto CanBeSuper(char input) -> bool {
    return kSuper[input] != nullptr;
}

}  // namespace

auto HandleLatexText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len)
    -> void {
    std::string latex{raw_text, len};
    for (size_t idx{0}; idx < latex.length(); ++idx) {
        if (latex[idx] == '_' || latex[idx] == '^') {
            bool is_super{latex[idx] == '^'};
            if (++idx >= latex.length()) {
                break;
            }
            if (latex[idx] == '{') {
                std::string super_sub{};
                for (; ++idx < latex.length();) {
                    if (latex[idx] == '}' && latex[idx - 1] != '\\') {
                        break;
                    }
                    super_sub += latex[idx];
                }
                if (std::all_of(super_sub.begin(), super_sub.end(),
                                [is_super](auto chr) {
                                    return is_super ? CanBeSuper(chr)
                                                    : CanBeSub(chr);
                                })) {
                    text += is_super ? ConvertTexSuper(super_sub)
                                     : ConvertTexSub(super_sub);
                } else {
                    text += is_super ? '^' : '_';
                    text += "{" + super_sub + '}';
                }
            } else {
                if (is_super ? CanBeSuper(latex[idx]) : CanBeSub(latex[idx])) {
                    text += is_super ? ConvertTexSuper(std::string{latex[idx]})
                                     : ConvertTexSub(std::string{latex[idx]});
                } else {
                    text += is_super ? '^' : '_';
                    text += latex[idx];
                }
            }
        } else {
            text += latex[idx];
        }
    }
}

auto HandleEntityText(std::string& text, const MD_CHAR* raw_text, MD_SIZE len)
    -> void {
    std::string raw{raw_text, len};
    if (raw == "&emsp;") {
        text += "    ";
    } else if (raw == "&nbsp;") {
        text += ' ';
    }
}

}  // namespace see::md2ansi
