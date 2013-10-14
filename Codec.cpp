// Codec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Codec.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcCodec.h"
#include "ppbox/avcodec/mpa/MpaCodec.h"
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
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            Codec * codec = factory_type::create(info.sub_type, ec);
            if (codec) {
                bool b = codec->finish_stream_info(info, ec);
                delete codec;
                return b;
            }
            return false;
        }

    } // namespace avcodec
} // namespace ppbox
