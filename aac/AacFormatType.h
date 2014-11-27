// AacFormatType.h

#ifndef _JUST_AVCODEC_AAC_AAC_FORMAT_TYPE_H_
#define _JUST_AVCODEC_AAC_AAC_FORMAT_TYPE_H_

#include "just/avcodec/AudioType.h"

namespace just
{
    namespace avcodec
    {

        struct AacFormatType
        {
            enum Enum
            {
                raw = 1, 
                adts, 
            };
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_FORMAT_TYPE_H_
