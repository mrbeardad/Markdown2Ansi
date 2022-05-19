#include "md2ansi/entity.hpp"

#include <boost/locale/encoding_utf.hpp>
#include <string>

extern "C" {
#include "entity.h"
}
#include "md4c.h"

#include "md2ansi/md2ansi_text.hpp"

namespace see::md2ansi {

EntityText::EntityText() : TextHandler{MD_TEXT_ENTITY} {}

void EntityText::HandleText(std::string& text, const MD_CHAR* raw, MD_SIZE len) {
  /* We assume UTF-8 output is what is desired. */
  if (len > 3 && raw[1] == '#') {
    char32_t codepoint = 0;

    if (raw[2] == 'x' || raw[2] == 'X') {
      /* Hexadecimal entity (e.g. "&#x1234abcd;")). */
      for (size_t i = 3; i < len - 1; i++) {
        unsigned hex_val{};
        auto chr = raw[i];
        if ('0' <= chr && chr <= '9') {
          hex_val = chr - '0';
        } else if ('A' <= chr && chr <= 'Z') {
          hex_val = chr - 'A' + 10;
        } else {
          hex_val = chr - 'a' + 10;
        }
        codepoint = 16 * codepoint + hex_val;
      }
    } else {
      /* Decimal entity (e.g. "&1234;") */
      for (size_t i = 2; i < len - 1; i++) {
        codepoint = 10 * codepoint + (raw[i] - '0');
      }
    }

    text += boost::locale::conv::utf_to_utf<char>(&codepoint, &codepoint + 1);
  } else {
    /* Named entity (e.g. "&nbsp;"). */
    const struct entity* ent{};

    ent = entity_lookup(raw, len);
    if (ent != nullptr) {
      text += boost::locale::conv::utf_to_utf<char>(ent->codepoints, ent->codepoints + 1);
      if (ent->codepoints[1]) {
        text += boost::locale::conv::utf_to_utf<char>(ent->codepoints + 1, ent->codepoints + 2);
      }
      return;
    }
  }

  text.append(raw, len);
}

}  // namespace see::md2ansi
