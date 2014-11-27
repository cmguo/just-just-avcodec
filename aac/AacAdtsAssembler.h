// AacAdtsAssembler.h

#ifndef _JUST_AVCODEC_AAC_AAC_ADTS_ASSEMBLER_H_
#define _JUST_AVCODEC_AAC_AAC_ADTS_ASSEMBLER_H_

#include "just/avcodec/Assembler.h"
#include "just/avcodec/aac/AacFormatType.h"

namespace just
{
    namespace avcodec
    {

        class AacAdtsAssembler
            : public Assembler
        {
        public:
            AacAdtsAssembler();

            virtual ~AacAdtsAssembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            std::vector<boost::uint8_t> adts_header_;
        };

        JUST_REGISTER_ASSEMBLER(AudioType::AAC,  AacFormatType::adts, AacAdtsAssembler);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_ADTS_ASSEMBLER_H_
