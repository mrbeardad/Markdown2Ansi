#ifndef SEE_MD2ANSI_ENTITY_HPP_
#define SEE_MD2ANSI_ENTITY_HPP_

#include "md2ansi/md2ansi_text.hpp"

namespace see::md2ansi {

class EntityText : public TextHandler {
 public:
  EntityText();
  EntityText(EntityText&&) = default;
  EntityText(const EntityText&) = default;
  auto operator=(EntityText&&) -> EntityText& = default;
  auto operator=(const EntityText&) -> EntityText& = default;
  ~EntityText() override = default;

  void HandleText(std::string& text, const MD_CHAR* raw, MD_SIZE len) override;
};

}  // namespace see::md2ansi

#endif  // !SEE_MD2ANSI_ENTITY_HPP_
