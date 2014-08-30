// AvcPacketAssembler.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_PACKET_ASSEMBLER_H_
#define _PPBOX_AVCODEC_AVC_AVC_PACKET_ASSEMBLER_H_

#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"

namespace ppbox
{
    namespace avcodec
    {

        class AvcPacketAssembler
            : public Assembler
        {
        public:
            AvcPacketAssembler();

            virtual ~AvcPacketAssembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec);
        };

        PPBOX_REGISTER_ASSEMBLER(VideoSubType::AVC, AvcFormatType::packet, AvcPacketAssembler);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_PACKET_ASSEMBLER_H_
