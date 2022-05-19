#include "md2ansi/normal.hpp"

#include "md4c.h"

#include "md2ansi/md2ansi_text.hpp"

namespace see::md2ansi {

NormalText::NormalText() : TextHandler{MD_TEXT_NORMAL} {}

}  // namespace see::md2ansi
