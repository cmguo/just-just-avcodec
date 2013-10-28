// Codec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Codec.h"
#include "ppbox/avcodec/Error.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcCodec.h"
#include "ppbox/avcodec/mpa/MpaCodec.h"
#include "ppbox/avcodec/aac/AacCodec.h"
//#include "ppbox/avcodec/ffmpeg/FFMpegCodec.h"

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
            Codec * codec = CodecFactory::create(info.sub_type, ec);
            if (codec) {
                bool b = codec->finish_stream_info(info, ec);
                delete codec;
                return b;
            }
            return false;
        }

        boost::system::error_code CodecTraits::error_not_found()
        {
            return error::codec_not_support;
        }

    } // namespace avcodec
} // namespace ppbox
