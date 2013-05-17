// AvcCodec.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_CODEC_H_
#define _PPBOX_AVCODEC_AVC_AVC_CODEC_H_

#include "ppbox/avcodec/Codec.h"

namespace ppbox
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
                StreamInfo & info);
        };

        PPBOX_REGISTER_CODEC(VideoSubType::AVC1, AvcCodec);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_CODEC_H_
