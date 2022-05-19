#include "md2ansi/del.hpp"

#include "md4c.h"

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

DelSpan::DelSpan() : SpanHandler{MD_SPAN_DEL, "\033[9m"} {}

}  // namespace see::md2ansi
