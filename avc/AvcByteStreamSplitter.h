// AvcByteStreamSplitter.h

#ifndef _JUST_AVCODEC_AVC_AVC_BYTE_STREAM_SPLITTER_H_
#define _JUST_AVCODEC_AVC_AVC_BYTE_STREAM_SPLITTER_H_

#include "just/avcodec/Splitter.h"
#include "just/avcodec/avc/AvcConfigHelper.h"
#include "just/avcodec/avc/AvcFormatType.h"
#include "just/avcodec/nalu/NaluHelper.h"

namespace just
{
    namespace avcodec
    {

        class AvcByteStreamSplitter
            : public Splitter
        {
        public:
            AvcByteStreamSplitter();

            virtual ~AvcByteStreamSplitter();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool split(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            just::avcodec::AvcConfigHelper config_;
            just::avcodec::NaluHelper helper_;
        };

        JUST_REGISTER_SPLITTER(VideoType::AVC, AvcFormatType::byte_stream, AvcByteStreamSplitter);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_BYTE_STREAM_SPLITTER_H_
