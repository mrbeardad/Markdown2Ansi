#include "md2ansi/md2ansi.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "md4c.h"

#include "md2ansi/block_code.hpp"
#include "md2ansi/block_h.hpp"
#include "md2ansi/block_hr.hpp"
#include "md2ansi/block_html.hpp"
#include "md2ansi/block_list.hpp"
#include "md2ansi/block_p.hpp"
#include "md2ansi/block_quote.hpp"
#include "md2ansi/block_table.hpp"
#include "md2ansi/span.hpp"

namespace see::md2ansi {

namespace {

// NOLINTNEXTLINE: reset each time
std::string kText{};
// NOLINTNEXTLINE: reset each time
std::vector<MD_BLOCKTYPE> kBlockContextStack{};
// NOLINTNEXTLINE: reset each time
bool kBlockCloseJustNow{false};

auto EnterBlock(MD_BLOCKTYPE type, void* detail, void* /*userdata*/) -> int {
    kBlockContextStack.emplace_back(type);
    // block element occupy all the width of parent element
    if (!kText.empty()) {
        if (kText.back() == '\x01') {
            kText.pop_back();
        } else if (kText.back() != '\n') {
            kText += '\n';
        }
    }
    switch (type) {
        case MD_BLOCK_DOC:
            break;
        case MD_BLOCK_QUOTE:
            HandleQuoteStart(kText);
            break;
        case MD_BLOCK_UL:
            HandleUlStart(kText, detail);
            break;
        case MD_BLOCK_OL:
            HandleOlStart(kText, detail);
            break;
        case MD_BLOCK_LI:
            HandleLiStart(kText, detail);
            break;
        case MD_BLOCK_HR:
            HandleHrStart(kText);
            break;
        case MD_BLOCK_H:
            HandleHStart(kText, detail);
            break;
        case MD_BLOCK_CODE:
            HandleCodeBlockStart(kText, detail);
            break;
        case MD_BLOCK_HTML:
            HandleHtmlStart(kText);
            break;
        case MD_BLOCK_P:
            HandlePStart(kText);
            break;
        case MD_BLOCK_TABLE:
            HandleTableStart(kText, detail);
            break;
        case MD_BLOCK_THEAD:
            HandleTheadStart(kText);
            break;
        case MD_BLOCK_TBODY:
            HandleTbodyStart(kText);
            break;
        case MD_BLOCK_TR:
            HandleTrStart(kText);
            break;
        case MD_BLOCK_TH:
            HandleThStart(kText, detail);
            break;
        case MD_BLOCK_TD:
            HandleTdStart(kText, detail);
            break;
    }
    return 0;
}

auto LeaveBlock(MD_BLOCKTYPE type, void* detail, void* /*userdata*/) -> int {
    switch (type) {
        case MD_BLOCK_DOC:
            break;
        case MD_BLOCK_QUOTE:
            HandleQuoteEnd(kText);
            break;
        case MD_BLOCK_UL:
            HandleUlEnd(kText, detail);
            break;
        case MD_BLOCK_OL:
            HandleOlEnd(kText, detail);
            break;
        case MD_BLOCK_LI:
            HandleLiEnd(kText, detail);
            break;
        case MD_BLOCK_HR:
            HandleHrEnd(kText);
            break;
        case MD_BLOCK_H:
            HandleHEnd(kText, detail);
            break;
        case MD_BLOCK_CODE:
            HandleCodeBlockEnd(kText, detail);
            break;
        case MD_BLOCK_HTML:
            HandleHtmlEnd(kText);
            break;
        case MD_BLOCK_P:
            HandlePEnd(kText);
            break;
        case MD_BLOCK_TABLE:
            HandleTableEnd(kText, detail);
            break;
        case MD_BLOCK_THEAD:
            HandleTheadEnd(kText);
            break;
        case MD_BLOCK_TBODY:
            HandleTbodyEnd(kText);
            break;
        case MD_BLOCK_TR:
            HandleTrEnd(kText);
            break;
        case MD_BLOCK_TH:
            HandleThEnd(kText, detail);
            break;
        case MD_BLOCK_TD:
            HandleTdEnd(kText, detail);
            break;
    }
    if (!kBlockCloseJustNow) {
        kText += '\n';
        kBlockCloseJustNow = true;
    }
    kBlockContextStack.pop_back();
    return 0;
}

auto EnterSpan(MD_SPANTYPE type, void* detail, void* /*userdata*/) -> int {
    switch (type) {
        case MD_SPAN_EM:
            HandleEmStart(kText);
            break;
        case MD_SPAN_STRONG:
            HandleStrongStart(kText);
            break;
        case MD_SPAN_A:
            HandleAStart(kText, detail);
            break;
        case MD_SPAN_IMG:
            HandleImgStart(kText, detail);
            break;
        case MD_SPAN_DEL:
            HandleDelStart(kText);
            break;
        case MD_SPAN_CODE:
            HandleCodeStart(kText);
            break;
        case MD_SPAN_U:
            HandleUStart(kText);
            break;
        case MD_SPAN_LATEXMATH:
            HandleLatexStart(kText);
            break;
        case MD_SPAN_LATEXMATH_DISPLAY:
            HandleLatexDisplayStart(kText);
            break;
        case MD_SPAN_WIKILINK:
            break;
    }
    return 0;
}

auto LeaveSpan(MD_SPANTYPE type, void* detail, void* /*userdata*/) -> int {
    switch (type) {
        case MD_SPAN_EM:
            HandleEmEnd(kText);
            break;
        case MD_SPAN_STRONG:
            HandleStrongEnd(kText);
            break;
        case MD_SPAN_A:
            HandleAEnd(kText, detail);
            break;
        case MD_SPAN_IMG:
            HandleImgEnd(kText, detail);
            break;
        case MD_SPAN_DEL:
            HandleDelEnd(kText);
            break;
        case MD_SPAN_CODE:
            HandleCodeEnd(kText);
            break;
        case MD_SPAN_U:
            HandleUEnd(kText);
            break;
        case MD_SPAN_LATEXMATH:
            HandleLatexEnd(kText);
            break;
        case MD_SPAN_LATEXMATH_DISPLAY:
            HandleLatexDisplayEnd(kText);
            break;
        case MD_SPAN_WIKILINK:
            break;
    }
    if (std::find(kBlockContextStack.begin(), kBlockContextStack.end(),
                  MD_BLOCK_QUOTE) != kBlockContextStack.end()) {
        kText += "\033[1;30m";
    }
    return 0;
}

auto TextHandle(MD_TEXTTYPE type,
                const MD_CHAR* text,
                MD_SIZE len,
                void* /*userdata*/) -> int {
    kBlockCloseJustNow = false;
    switch (type) {
        case MD_TEXT_NORMAL:
            kText.append(text, len);
            break;
        case MD_TEXT_BR:
            kText += '\n';
            break;
        case MD_TEXT_ENTITY:
            HandleEntityText(kText, text, len);
            break;
        case MD_TEXT_CODE:
            HandleCodeBlockText(kText, text, len);
            break;
        case MD_TEXT_HTML:
            HandleHtmlText(kText, text, len);
            break;
        case MD_TEXT_LATEXMATH:
            HandleLatexText(kText, text, len);
        case MD_TEXT_NULLCHAR:
        case MD_TEXT_SOFTBR:
            break;
    }
    return 0;
}

}  // namespace

#define MD_CUSTOM_FLAGS                                                   \
    MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES | MD_FLAG_STRIKETHROUGH | \
        MD_FLAG_TASKLISTS | MD_FLAG_LATEXMATHSPANS;

auto Highlight(const std::string& text) -> std::string {
    MD_PARSER parser{};
    parser.flags = MD_CUSTOM_FLAGS;
    parser.enter_block = EnterBlock;
    parser.leave_block = LeaveBlock;
    parser.enter_span = EnterSpan;
    parser.leave_span = LeaveSpan;
    parser.text = TextHandle;

    md_parse(text.c_str(), text.length(), &parser, nullptr);

    auto ret = std::move(kText);
    kText.clear();
    return ret;
}

}  // namespace see::md2ansi
