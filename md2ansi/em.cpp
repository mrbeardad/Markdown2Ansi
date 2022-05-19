#include "md2ansi/em.hpp"

#include "md4c.h"

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

EmSpan::EmSpan() : SpanHandler{MD_SPAN_EM, "\033[3m"} {}

}  // namespace see::md2ansi
