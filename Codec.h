// Codec.h

#ifndef _PPBOX_AVCODEC_CODEC_H_
#define _PPBOX_AVCODEC_CODEC_H_

#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/StreamInfo.h"

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Codec
            : public ppbox::common::ClassFactory<
                Codec, 
                boost::uint32_t, 
                Codec *()
            >
        {
        public:
            Codec();

            virtual ~Codec();

        public:
            virtual bool finish_stream_info(
                StreamInfo & info) = 0;

        public:
            static bool static_finish_stream_info(
                StreamInfo & info);
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_CODEC(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVCODEC_CODEC_H_
