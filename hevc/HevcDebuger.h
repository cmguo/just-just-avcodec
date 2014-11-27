// HevcDebuger.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_DEBUGER_H_
#define _JUST_HEVCODEC_HEVC_HEVC_DEBUGER_H_

#include "just/avcodec/Debuger.h"
#include "just/avcodec/VideoType.h"

namespace just
{
    namespace avcodec
    {

        class HevcDebuger
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

        JUST_REGISTER_DEBUGER(VideoType::HEVC, HevcDebuger);

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_DEBUGER_H_
