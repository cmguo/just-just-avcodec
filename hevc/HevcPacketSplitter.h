// HevcPacketSplitter.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_PACKET_SPLITTER_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_PACKET_SPLITTER_H_

#include "ppbox/avcodec/Splitter.h"
#include "ppbox/avcodec/hevc/HevcConfigHelper.h"
#include "ppbox/avcodec/hevc/HevcFormatType.h"
#include "ppbox/avcodec/nalu/NaluHelper.h"

namespace ppbox
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
            ppbox::avcodec::HevcConfigHelper config_;
            ppbox::avcodec::NaluHelper helper_;
        };

        PPBOX_REGISTER_SPLITTER(VideoType::HEVC, HevcFormatType::packet, HevcPacketSplitter);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_PACKET_SPLITTER_H_
