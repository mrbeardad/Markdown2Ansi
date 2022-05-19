#include "md2ansi/u.hpp"

#include "md4c.h"

namespace see::md2ansi {

USpan::USpan() : SpanHandler(MD_SPAN_U, "\033[4m") {}

}  // namespace see::md2ansi
