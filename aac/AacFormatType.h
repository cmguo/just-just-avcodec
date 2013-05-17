// AacFormatType.h

#ifndef _PPBOX_AVCODEC_AAC_AAC_FORMAT_TYPE_H_
#define _PPBOX_AVCODEC_AAC_AAC_FORMAT_TYPE_H_

#include "ppbox/avcodec/CodecType.h"

namespace ppbox
{
    namespace avcodec
    {

        struct AacFormatType
        {
            enum Enum
            {
                raw = 0, 
                adts, 
            };
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AAC_AAC_FORMAT_TYPE_H_
