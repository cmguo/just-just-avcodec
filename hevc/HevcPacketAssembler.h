// HevcPacketAssembler.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_PACKET_ASSEMBLER_H_
#define _JUST_HEVCODEC_HEVC_HEVC_PACKET_ASSEMBLER_H_

#include "just/avcodec/Assembler.h"
#include "just/avcodec/hevc/HevcFormatType.h"

namespace just
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

        JUST_REGISTER_ASSEMBLER(VideoType::HEVC, HevcFormatType::packet, HevcPacketAssembler);

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_PACKET_ASSEMBLER_H_
