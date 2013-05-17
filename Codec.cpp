// Codec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Codec.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcCodec.h"
#include "ppbox/avcodec/aac/AacCodec.h"

namespace ppbox
{
    namespace avcodec
    {

        Codec::Codec()
        {
        }

        Codec::~Codec()
        {
        }

        bool Codec::static_finish_stream_info(
            StreamInfo & info)
        {
            Codec * codec = factory_type::create(info.sub_type);
            if (codec) {
                codec->finish_stream_info(info);
                delete codec;
                return true;
            }
            return false;
        }

    } // namespace avcodec
} // namespace ppbox
