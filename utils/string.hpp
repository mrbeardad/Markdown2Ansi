#ifndef SEE_UTILS_STRING_HPP_
#define SEE_UTILS_STRING_HPP_

#include <string>

template <typename CharT, typename StrT>
auto operator<<(std::basic_string<CharT>& lhs, const StrT& rhs)
    -> std::basic_string<CharT>& {
    return lhs.append(rhs);
}

template <typename CharT>
auto operator<<(std::basic_string<CharT>& output, CharT onechar)
    -> std::basic_string<CharT>& {
    output.push_back(onechar);
    return output;
}

#endif  // !SEE_UTILS_STRING_HPP_
