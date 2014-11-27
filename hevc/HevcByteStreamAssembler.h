// HevcByteStreamAssembler.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_BYTE_STREAM_ASSEMBLER_H_
#define _JUST_HEVCODEC_HEVC_HEVC_BYTE_STREAM_ASSEMBLER_H_

#include "just/avcodec/Assembler.h"
#include "just/avcodec/hevc/HevcFormatType.h"

namespace just
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

        JUST_REGISTER_ASSEMBLER(VideoType::HEVC, HevcFormatType::byte_stream, HevcByteStreamAssembler);

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_BYTE_STREAM_ASSEMBLER_H_
