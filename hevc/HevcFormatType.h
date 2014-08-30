// HevcFormatType.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_FORMAT_TYPE_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_FORMAT_TYPE_H_

#include "ppbox/avcodec/CodecType.h"

namespace ppbox
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
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_FORMAT_TYPE_H_
