#include "md2ansi/strong.hpp"
#include "md2ansi/md2ansi_span.hpp"
#include "md4c.h"

namespace see::md2ansi {

StrongSpan::StrongSpan() : SpanHandler{MD_SPAN_STRONG, "\033[1m"} {}

}  // namespace see::md2ansi
