// HevcCodec.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_CODEC_H_
#define _JUST_HEVCODEC_HEVC_HEVC_CODEC_H_

#include "just/avcodec/Codec.h"

namespace just
{
    namespace avcodec
    {

        class HevcCodec
            : public Codec
        {
        public:
            HevcCodec();

        public:
            bool finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec);
        };

        JUST_REGISTER_CODEC(VideoType::HEVC, HevcCodec);

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_CODEC_H_
