// HevcByteStreamSplitter.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_BYTE_STREAM_SPLITTER_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_BYTE_STREAM_SPLITTER_H_

#include "ppbox/avcodec/Splitter.h"
#include "ppbox/avcodec/hevc/HevcConfigHelper.h"
#include "ppbox/avcodec/hevc/HevcFormatType.h"
#include "ppbox/avcodec/nalu/NaluHelper.h"

namespace ppbox
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
            ppbox::avcodec::HevcConfigHelper config_;
            ppbox::avcodec::NaluHelper helper_;
        };

        PPBOX_REGISTER_SPLITTER(VideoSubType::HEVC, HevcFormatType::byte_stream, HevcByteStreamSplitter);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_BYTE_STREAM_SPLITTER_H_
