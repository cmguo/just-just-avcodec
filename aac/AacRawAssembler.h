// AacRawAssembler.h

#ifndef _JUST_AVCODEC_AAC_AAC_RAW_ASSEMBLER_H_
#define _JUST_AVCODEC_AAC_AAC_RAW_ASSEMBLER_H_

#include "just/avcodec/Assembler.h"
#include "just/avcodec/aac/AacFormatType.h"

namespace just
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

        JUST_REGISTER_ASSEMBLER(AudioType::AAC,  AacFormatType::raw, AacRawAssembler);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_RAW_ASSEMBLER_H_
