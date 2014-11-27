// AacDebuger.h

#ifndef _JUST_AACODEC_AAC_AAC_DEBUGER_H_
#define _JUST_AACODEC_AAC_AAC_DEBUGER_H_

#include "just/avcodec/Debuger.h"
#include "just/avcodec/AudioType.h"

namespace just
{
    namespace avcodec
    {

        class AacDebuger
            : public Debuger
        {
        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool debug(
                Sample & sample, 
                boost::system::error_code & ec);

        private:
            std::vector<boost::uint8_t> adts_header_;
        };

        JUST_REGISTER_DEBUGER(AudioType::AAC,  AacDebuger);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AACODEC_AAC_AAC_DEBUGER_H_
