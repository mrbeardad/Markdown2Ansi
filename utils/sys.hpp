#ifndef SEE_UTILS_SYS_HPP_
#define SEE_UTILS_SYS_HPP_

#include <system_error>

namespace see::utils {

template <typename T>
auto HandleSyscall(T sysCallRet) -> T {
  if (sysCallRet < 0) {
    throw std::system_error{errno, std::generic_category()};
  }
  return sysCallRet;
}

template <typename T>
auto HandleSyscall(T* sysCallRet) -> T* {
  if (sysCallRet == nullptr) {
    throw std::system_error{errno, std::generic_category()};
  }
  return sysCallRet;
}

}  // namespace see::utils

#endif  // !SEE_UTILS_SYS_HPP_
