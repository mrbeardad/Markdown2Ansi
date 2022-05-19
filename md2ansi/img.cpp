#include "md2ansi/img.hpp"
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include "md2ansi/md2ansi_span.hpp"
#include "md4c.h"

namespace see::md2ansi {

ImgSpan::ImgSpan() : SpanHandler(MD_SPAN_IMG, "\033[38;2;0;152;245;4m"), viu_executed_{false} {}

void ImgSpan::SpanStart(std::string& text, void* detail) {
  auto img_detail = reinterpret_cast<MD_SPAN_IMG_DETAIL*>(detail);

  viu_executed_ = false;
  if (std::system("which viu >/dev/null") == 0) {
    static std::regex kUrl{R"(^(([^:/?#]+):)(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)"};
    std::string src{img_detail->src.text, img_detail->src.size};
    if (!std::regex_match(src, kUrl) && std::filesystem::exists(src)) {
      viu_executed_ = true;
      text += '\n';
      auto cmd = "viu " + src + " 2>/dev/null";
      auto output = ::popen(cmd.c_str(), "r");
      for (int chr{}; (chr = std::fgetc(output)) != EOF;) {
        text += static_cast<char>(chr);
      }
    }
  }

  text += " \033[31m!\033[37m[" + GetStyle();
}

void ImgSpan::SpanEnd(std::string& text, void* detail) {
  auto img_detail = reinterpret_cast<MD_SPAN_IMG_DETAIL*>(detail);

  text += "\033[24;37m](" + GetStyle();
  text.append(img_detail->src.text, img_detail->src.size);
  text += "\033[24m";
  if (img_detail->title.size != 0) {
    text += " \033[2m\"";
    text.append(img_detail->title.text, img_detail->title.size);
    text += "\"\033[22m";
  }
  text += "\033[37m) ";

  if (viu_executed_) {
    text += '\n';
  }
}

}  // namespace see::md2ansi
