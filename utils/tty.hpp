#ifndef SEE_UTILS_TTY_HPP_
#define SEE_UTILS_TTY_HPP_

#include <sys/ioctl.h>
#include <unistd.h>

#include <utility>

namespace see::utils {

inline auto GetTtyWinSize() -> ::winsize {
    static ::winsize kWinSize{};
    static bool kIsInited{false};

    if (!kIsInited) {
        ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &kWinSize);
    }

    return kWinSize;
}

}  // namespace see::utils

#endif  // !SEE_UTILS_TTY_HPP_
