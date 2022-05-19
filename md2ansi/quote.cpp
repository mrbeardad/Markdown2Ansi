#include "md2ansi/quote.hpp"
#include "md2ansi/md2ansi_block.hpp"
#include "md4c.h"

namespace see::md2ansi {

QuoteBlock::QuoteBlock() : BlockHandler(MD_BLOCK_QUOTE, "\033[2m") {}

}  // namespace see::md2ansi
