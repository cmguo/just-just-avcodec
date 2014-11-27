// AacRawSplitter.h

#ifndef _JUST_AVCODEC_AAC_AAC_RAW_SPLITTER_H_
#define _JUST_AVCODEC_AAC_AAC_RAW_SPLITTER_H_

#include "just/avcodec/Splitter.h"
#include "just/avcodec/aac/AacFormatType.h"
#include "just/avcodec/aac/AacConfigHelper.h"

namespace just
{
    namespace avcodec
    {

        class AacRawSplitter
            : public Splitter
        {
        public:
            AacRawSplitter();

            virtual ~AacRawSplitter();

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

        JUST_REGISTER_SPLITTER(AudioType::AAC,  AacFormatType::raw, AacRawSplitter);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_RAW_SPLITTER_H_
