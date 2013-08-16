// Assembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/Error.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avcodec/aac/AacAdtsAssembler.h"
#include "ppbox/avcodec/avc/AvcPacketAssembler.h"
#include "ppbox/avcodec/avc/AvcByteStreamAssembler.h"

namespace ppbox
{
    namespace avcodec
    {

        boost::system::error_code Assembler::error_not_found()
        {
            return error::assembler_not_support;
        }

        Assembler * Assembler::create(
            boost::uint32_t codec_type, 
            boost::uint32_t format, 
            boost::system::error_code & ec)
        {
            return factory_type::create(((boost::uint64_t)codec_type << 32) | format, ec);
        }

        Assembler::Assembler()
        {
        }

        Assembler::~Assembler()
        {
        }

    } // namespace avcodec
} // namespace ppbox
