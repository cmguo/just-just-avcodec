// HevcFormatType.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_FORMAT_TYPE_H_
#define _JUST_HEVCODEC_HEVC_HEVC_FORMAT_TYPE_H_

#include "just/avcodec/VideoType.h"

namespace just
{
    namespace avcodec
    {

        struct HevcFormatType
        {
            enum Enum
            {
                packet = 1, 
                byte_stream = 2, 
            };
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_FORMAT_TYPE_H_
