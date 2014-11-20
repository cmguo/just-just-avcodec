// AvcFormatType.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_FORMAT_TYPE_H_
#define _PPBOX_AVCODEC_AVC_AVC_FORMAT_TYPE_H_

#include "ppbox/avcodec/VideoType.h"

namespace ppbox
{
    namespace avcodec
    {

        struct AvcFormatType
        {
            enum Enum
            {
                packet = 1, 
                byte_stream = 2, 
            };
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_FORMAT_TYPE_H_
