// HevcDebuger.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_DEBUGER_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_DEBUGER_H_

#include "ppbox/avcodec/Debuger.h"
#include "ppbox/avcodec/CodecType.h"

namespace ppbox
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

        PPBOX_REGISTER_DEBUGER(VideoSubType::HEVC, HevcDebuger);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_DEBUGER_H_
