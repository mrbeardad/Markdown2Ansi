#ifndef SEE_UTILS_STRING_HPP_
#define SEE_UTILS_STRING_HPP_

#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

template <typename CharT, typename StrOrCharT>
auto operator<<(std::basic_string<CharT>& lhs, StrOrCharT&& rhs) -> std::basic_string<CharT>& {
  if constexpr (std::is_integral_v<std::decay_t<StrOrCharT>>) {
    lhs.push_back(rhs);
    return lhs;
  } else {
    return lhs.append(std::forward<StrOrCharT>(rhs));
  }
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
