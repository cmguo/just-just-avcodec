// SubtlType.h

#ifndef _PPBOX_AVCODEC_SUBTL_TYPE_H_
#define _PPBOX_AVCODEC_SUBTL_TYPE_H_

#include "ppbox/avcodec/Base.h"

namespace ppbox
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
} // namespace ppbox

#endif // _PPBOX_AVCODEC_SUBTL_TYPE_H_
