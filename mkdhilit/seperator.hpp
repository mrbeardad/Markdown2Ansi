#ifndef MRBEARDAD_SEE_SEPERATOR_HPP
#define MRBEARDAD_SEE_SEPERATOR_HPP

#include "mkdblock.hpp"

namespace see::mkdhilit {

class SeperatorBlk : public MkdBlock {
    static SeperatorBlk Instance_;

   public:
    SeperatorBlk();
    bool matchBegin(const std::string& oneline) override;
    bool matchEnd(const std::string&) override;
    std::string& highlight(std::string& text) override;
};
inline SeperatorBlk SeperatorBlk::Instance_{};

}  // namespace see::mkdhilit

#endif  // MRBEARDAD_SEE_SEPERATOR_HPP
