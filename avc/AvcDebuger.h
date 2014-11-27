// AvcDebuger.h

#ifndef _JUST_AVCODEC_AVC_AVC_DEBUGER_H_
#define _JUST_AVCODEC_AVC_AVC_DEBUGER_H_

#include "just/avcodec/Debuger.h"
#include "just/avcodec/VideoType.h"

namespace just
{
    namespace avcodec
    {

        class AvcDebuger
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

        JUST_REGISTER_DEBUGER(VideoType::AVC, AvcDebuger);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_DEBUGER_H_
