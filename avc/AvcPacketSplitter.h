// AvcPacketSplitter.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_PACKET_SPLITTER_H_
#define _PPBOX_AVCODEC_AVC_AVC_PACKET_SPLITTER_H_

#include "ppbox/avcodec/Splitter.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"
#include "ppbox/avcodec/avc/AvcNaluHelper.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"

namespace ppbox
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
            ppbox::avcodec::AvcConfigHelper config_;
            ppbox::avcodec::AvcNaluHelper helper_;
        };

        PPBOX_REGISTER_SPLITTER(VideoSubType::AVC1, AvcFormatType::packet, AvcPacketSplitter);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_PACKET_SPLITTER_H_
