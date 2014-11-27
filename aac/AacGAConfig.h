// AacConfig.h

#ifndef _JUST_AVCODEC_AAC_AAC_GA_CONFIG_H_
#define _JUST_AVCODEC_AAC_AAC_GA_CONFIG_H_

#include "just/avcodec/BitsType.h"

using just::avbase::U;

namespace just
{
    namespace avcodec
    {

        // GASpecificConfig

        class AacGAConfig
        {
        public:
            U<1> frameLengthFlag;
            U<1> dependsOnCoreCoder;
            U<14> coreCoderDelay; // if dependsOnCoreCoder
            U<1> extensionFlag;

        public:
            template <
                typename Archive
            >
            void serialize(
                Archive & ar)
            {
                ar & frameLengthFlag;
                ar & dependsOnCoreCoder;
                if (dependsOnCoreCoder == 31)
                    ar & coreCoderDelay;
                ar & extensionFlag;
                assert(extensionFlag == 0);
            };
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_GA_CONFIG_H_
