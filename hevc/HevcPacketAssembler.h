// HevcPacketAssembler.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_PACKET_ASSEMBLER_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_PACKET_ASSEMBLER_H_

#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/hevc/HevcFormatType.h"

namespace ppbox
{
    namespace avcodec
    {

        class HevcPacketAssembler
            : public Assembler
        {
        public:
            HevcPacketAssembler();

            virtual ~HevcPacketAssembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec);
        };

        PPBOX_REGISTER_ASSEMBLER(VideoSubType::HEVC, HevcFormatType::packet, HevcPacketAssembler);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_PACKET_ASSEMBLER_H_
