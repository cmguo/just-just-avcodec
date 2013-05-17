// Splitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Splitter.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avcodec/aac/AacAdtsSplitter.h"
#include "ppbox/avcodec/avc/AvcPacketSplitter.h"
#include "ppbox/avcodec/avc/AvcByteStreamSplitter.h"

namespace ppbox
{
    namespace avcodec
    {

        Splitter::Splitter()
        {
        }

        Splitter::~Splitter()
        {
        }

        Splitter * Splitter::create(
            boost::uint32_t codec_type, 
            boost::uint32_t format)
        {
            return factory_type::create(((boost::uint64_t)codec_type << 32) | format);
        }

    } // namespace avcodec
} // namespace ppbox
