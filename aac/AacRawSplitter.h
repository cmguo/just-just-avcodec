// AacRawSplitter.h

#ifndef _PPBOX_AVCODEC_AAC_AAC_RAW_SPLITTER_H_
#define _PPBOX_AVCODEC_AAC_AAC_RAW_SPLITTER_H_

#include "ppbox/avcodec/Splitter.h"
#include "ppbox/avcodec/aac/AacFormatType.h"
#include "ppbox/avcodec/aac/AacConfigHelper.h"

namespace ppbox
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

        PPBOX_REGISTER_SPLITTER(AudioType::AAC,  AacFormatType::raw, AacRawSplitter);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AAC_AAC_RAW_SPLITTER_H_
