// AvcPacketAssembler.h

#ifndef _JUST_AVCODEC_AVC_AVC_PACKET_ASSEMBLER_H_
#define _JUST_AVCODEC_AVC_AVC_PACKET_ASSEMBLER_H_

#include "just/avcodec/Assembler.h"
#include "just/avcodec/avc/AvcFormatType.h"

namespace just
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

        JUST_REGISTER_ASSEMBLER(VideoType::AVC, AvcFormatType::packet, AvcPacketAssembler);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_PACKET_ASSEMBLER_H_
