// AacDebuger.h

#ifndef _PPBOX_AACODEC_AAC_AAC_DEBUGER_H_
#define _PPBOX_AACODEC_AAC_AAC_DEBUGER_H_

#include "ppbox/avcodec/Debuger.h"
#include "ppbox/avcodec/CodecType.h"

namespace ppbox
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
        };

        PPBOX_REGISTER_DEBUGER(AudioSubType::MP4A, AacDebuger);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AACODEC_AAC_AAC_DEBUGER_H_
