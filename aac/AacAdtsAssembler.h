// AacAdtsAssembler.h

#ifndef _PPBOX_AVCODEC_AAC_AAC_ADTS_ASSEMBLER_H_
#define _PPBOX_AVCODEC_AAC_AAC_ADTS_ASSEMBLER_H_

#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/aac/AacConfigHelper.h"
#include "ppbox/avcodec/aac/AacFormatType.h"

namespace ppbox
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
            AacConfigHelper config_;
        };

        PPBOX_REGISTER_ASSEMBLER(AudioSubType::MP4A, AacFormatType::adts, AacAdtsAssembler);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AAC_AAC_ADTS_ASSEMBLER_H_
