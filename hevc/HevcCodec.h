// HevcCodec.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_CODEC_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_CODEC_H_

#include "ppbox/avcodec/Codec.h"

namespace ppbox
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

        PPBOX_REGISTER_CODEC(VideoSubType::HEVC, HevcCodec);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_CODEC_H_
