// HevcByteStreamAssembler.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_BYTE_STREAM_ASSEMBLER_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_BYTE_STREAM_ASSEMBLER_H_

#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/hevc/HevcFormatType.h"

namespace ppbox
{
    namespace avcodec
    {

        class HevcByteStreamAssembler
            : public Assembler
        {
        public:
            HevcByteStreamAssembler();

            virtual ~HevcByteStreamAssembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            std::vector<boost::uint8_t> access_unit_delimiter_;
            std::vector<boost::uint8_t> sequence_header_;
            bool init_;
        };

        PPBOX_REGISTER_ASSEMBLER(VideoSubType::HEVC, HevcFormatType::byte_stream, HevcByteStreamAssembler);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_BYTE_STREAM_ASSEMBLER_H_
