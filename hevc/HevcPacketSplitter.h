// HevcPacketSplitter.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_PACKET_SPLITTER_H_
#define _JUST_HEVCODEC_HEVC_HEVC_PACKET_SPLITTER_H_

#include "just/avcodec/Splitter.h"
#include "just/avcodec/hevc/HevcConfigHelper.h"
#include "just/avcodec/hevc/HevcFormatType.h"
#include "just/avcodec/nalu/NaluHelper.h"

namespace just
{
    namespace avcodec
    {

        class HevcPacketSplitter
            : public Splitter
        {
        public:
            HevcPacketSplitter();

            virtual ~HevcPacketSplitter();

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

        JUST_REGISTER_SPLITTER(VideoType::HEVC, HevcFormatType::packet, HevcPacketSplitter);

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_PACKET_SPLITTER_H_
