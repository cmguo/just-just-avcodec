// AacAdtsSplitter.h

#ifndef _JUST_AVCODEC_AAC_AAC_ADTS_SPLITTER_H_
#define _JUST_AVCODEC_AAC_AAC_ADTS_SPLITTER_H_

#include "just/avcodec/Splitter.h"
#include "just/avcodec/aac/AacFormatType.h"
#include "just/avcodec/aac/AacConfigHelper.h"

namespace just
{
    namespace avcodec
    {

        class AacAdtsSplitter
            : public Splitter
        {
        public:
            AacAdtsSplitter();

            virtual ~AacAdtsSplitter();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool split(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            AacConfigHelper config_;
        };

        JUST_REGISTER_SPLITTER(AudioType::AAC,  AacFormatType::adts, AacAdtsSplitter);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_ADTS_SPLITTER_H_
