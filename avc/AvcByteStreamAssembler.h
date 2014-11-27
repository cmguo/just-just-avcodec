// AvcByteStreamAssembler.h

#ifndef _JUST_AVCODEC_AVC_AVC_BYTE_STREAM_ASSEMBLER_H_
#define _JUST_AVCODEC_AVC_AVC_BYTE_STREAM_ASSEMBLER_H_

#include "just/avcodec/Assembler.h"
#include "just/avcodec/avc/AvcFormatType.h"

namespace just
{
    namespace avcodec
    {

        class AvcByteStreamAssembler
            : public Assembler
        {
        public:
            AvcByteStreamAssembler();

            virtual ~AvcByteStreamAssembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            std::vector<boost::uint8_t> access_unit_delimiter_;
            std::vector<boost::uint8_t> sps_pps_;
            bool init_;
        };

        JUST_REGISTER_ASSEMBLER(VideoType::AVC, AvcFormatType::byte_stream, AvcByteStreamAssembler);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_BYTE_STREAM_ASSEMBLER_H_
