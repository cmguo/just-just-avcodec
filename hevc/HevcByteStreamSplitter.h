// HevcByteStreamSplitter.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_BYTE_STREAM_SPLITTER_H_
#define _JUST_HEVCODEC_HEVC_HEVC_BYTE_STREAM_SPLITTER_H_

#include "just/avcodec/Splitter.h"
#include "just/avcodec/hevc/HevcConfigHelper.h"
#include "just/avcodec/hevc/HevcFormatType.h"
#include "just/avcodec/nalu/NaluHelper.h"

namespace just
{
    namespace avcodec
    {

        class HevcByteStreamSplitter
            : public Splitter
        {
        public:
            HevcByteStreamSplitter();

            virtual ~HevcByteStreamSplitter();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool split(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            just::avcodec::HevcConfigHelper config_;
            just::avcodec::NaluHelper helper_;
        };

        JUST_REGISTER_SPLITTER(VideoType::HEVC, HevcFormatType::byte_stream, HevcByteStreamSplitter);

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_BYTE_STREAM_SPLITTER_H_
