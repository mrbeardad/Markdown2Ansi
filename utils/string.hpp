#ifndef SEE_UTILS_STRING_HPP_
#define SEE_UTILS_STRING_HPP_

#include <cstddef>
#include <string>
#include <utility>

template <typename CharT, typename StrT>
auto operator<<(std::basic_string<CharT>& lhs, StrT&& rhs)
    -> std::basic_string<CharT>& {
    return lhs.append(std::forward<StrT>(rhs));
}

template <typename CharT>
auto operator<<(std::basic_string<CharT>& output, CharT onechar)
    -> std::basic_string<CharT>& {
    output.push_back(onechar);
    return output;
}

template <typename StrT>
auto operator*(const StrT& str, int count) -> StrT {
    StrT ret{};
    for (int cnt{0}; cnt < count; ++cnt) {
        ret += str;
    }
    return ret;
}

template <typename CharT>
auto Repeat(const CharT* cstr, int count) -> std::basic_string<CharT> {
    std::string ret{};
    for (int cnt{0}; cnt < count; ++cnt) {
        ret += cstr;
    }
    return ret;
}

#endif  // !SEE_UTILS_STRING_HPP_
