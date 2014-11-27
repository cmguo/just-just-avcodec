// AvcCodec.h

#ifndef _JUST_AVCODEC_AVC_AVC_CODEC_H_
#define _JUST_AVCODEC_AVC_AVC_CODEC_H_

#include "just/avcodec/Codec.h"
#include "just/avcodec/VideoType.h"

namespace just
{
    namespace avcodec
    {

        class AvcCodec
            : public Codec
        {
        public:
            AvcCodec();

        public:
            bool finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec);
        };

        JUST_REGISTER_CODEC(VideoType::AVC, AvcCodec);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_CODEC_H_
