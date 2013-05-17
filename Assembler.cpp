// Assembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Assembler.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avcodec/aac/AacAdtsAssembler.h"
#include "ppbox/avcodec/avc/AvcPacketAssembler.h"
#include "ppbox/avcodec/avc/AvcByteStreamAssembler.h"

namespace ppbox
{
    namespace avcodec
    {

        Assembler::Assembler()
        {
        }

        Assembler::~Assembler()
        {
        }

        Assembler * Assembler::create(
            boost::uint32_t codec_type, 
            boost::uint32_t format)
        {
            return factory_type::create(((boost::uint64_t)codec_type << 32) | format);
        }

    } // namespace avcodec
} // namespace ppbox
