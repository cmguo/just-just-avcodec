// AvcDebuger.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_DEBUGER_H_
#define _PPBOX_AVCODEC_AVC_AVC_DEBUGER_H_

#include "ppbox/avcodec/Debuger.h"
#include "ppbox/avcodec/VideoType.h"

namespace ppbox
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

        PPBOX_REGISTER_DEBUGER(VideoType::AVC, AvcDebuger);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_DEBUGER_H_
