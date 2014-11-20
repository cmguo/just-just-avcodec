// AvcByteStreamAssembler.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_BYTE_STREAM_ASSEMBLER_H_
#define _PPBOX_AVCODEC_AVC_AVC_BYTE_STREAM_ASSEMBLER_H_

#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"

namespace ppbox
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

        PPBOX_REGISTER_ASSEMBLER(VideoType::AVC, AvcFormatType::byte_stream, AvcByteStreamAssembler);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_BYTE_STREAM_ASSEMBLER_H_
