#ifndef SEE_MD2ANSI_IMG_HPP_
#define SEE_MD2ANSI_IMG_HPP_

#include "md2ansi/md2ansi_span.hpp"

namespace see::md2ansi {

class ImgSpan : public SpanHandler {
 public:
  ImgSpan();
  ImgSpan(ImgSpan&&) = default;
  ImgSpan(const ImgSpan&) = default;
  auto operator=(ImgSpan&&) -> ImgSpan& = default;
  auto operator=(const ImgSpan&) -> ImgSpan& = default;
  ~ImgSpan() override = default;

  void SpanStart(std::string& text, void* detail) override;
  void SpanEnd(std::string& text, void* detail) override;

 private:
  bool viu_executed_;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_IMG_HPP_
