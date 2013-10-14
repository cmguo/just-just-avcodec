// AacRawAssembler.h

#ifndef _PPBOX_AVCODEC_AAC_AAC_RAW_ASSEMBLER_H_
#define _PPBOX_AVCODEC_AAC_AAC_RAW_ASSEMBLER_H_

#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/aac/AacFormatType.h"

namespace ppbox
{
    namespace avcodec
    {

        class AacRawAssembler
            : public Assembler
        {
        public:
            AacRawAssembler();

            virtual ~AacRawAssembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec);
        };

        PPBOX_REGISTER_ASSEMBLER(AudioSubType::MP4A, AacFormatType::raw, AacRawAssembler);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AAC_AAC_RAW_ASSEMBLER_H_
