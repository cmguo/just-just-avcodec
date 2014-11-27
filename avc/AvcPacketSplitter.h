// AvcPacketSplitter.h

#ifndef _JUST_AVCODEC_AVC_AVC_PACKET_SPLITTER_H_
#define _JUST_AVCODEC_AVC_AVC_PACKET_SPLITTER_H_

#include "just/avcodec/Splitter.h"
#include "just/avcodec/avc/AvcConfigHelper.h"
#include "just/avcodec/avc/AvcFormatType.h"
#include "just/avcodec/nalu/NaluHelper.h"

namespace just
{
    namespace avcodec
    {

        class AvcPacketSplitter
            : public Splitter
        {
        public:
            AvcPacketSplitter();

            virtual ~AvcPacketSplitter();

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

        JUST_REGISTER_SPLITTER(VideoType::AVC, AvcFormatType::packet, AvcPacketSplitter);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_PACKET_SPLITTER_H_
