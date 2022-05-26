#ifndef SEE_UTILS_COLOR_HPP_
#define SEE_UTILS_COLOR_HPP_

#include <string>

namespace see::utils {

/**
 * @brief Make text with rainbow foreground and background color
 *
 * @param text input text
 * @return std::string rainbow text
 */
auto RainbowText(const std::string& text) -> std::string;

}  // namespace see::utils

#endif  // !SEE_UTILS_COLOR_HPP_
