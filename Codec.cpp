// Codec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Codec.h"
#include "ppbox/avcodec/Error.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcCodec.h"
#include "ppbox/avcodec/hevc/HevcCodec.h"
#include "ppbox/avcodec/mpa/MpaCodec.h"
#include "ppbox/avcodec/aac/AacCodec.h"
#ifndef PPBOX_DISABLE_FFMPEG
#  include "ppbox/avcodec/ffmpeg/FFMpegCodec.h"
#endif

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
            ec.clear();
            return true;
        }

        boost::system::error_code CodecTraits::error_not_found()
        {
            return error::codec_not_support;
        }

    } // namespace avcodec
} // namespace ppbox
