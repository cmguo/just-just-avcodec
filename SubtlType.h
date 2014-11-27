// SubtlType.h

#ifndef _JUST_AVCODEC_SUBTL_TYPE_H_
#define _JUST_AVCODEC_SUBTL_TYPE_H_

#include "just/avcodec/Base.h"

namespace just
{
    namespace avcodec
    {

        struct SubtlType
        {
            enum Enum
            {
                UTF8 = MAKE_FOURC_TYPE('U', 'T', 'F', '8'), 
            };
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_SUBTL_TYPE_H_
