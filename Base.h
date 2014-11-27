// Base.h

#ifndef _JUST_AVCODEC_BASE_H_
#define _JUST_AVCODEC_BASE_H_

#include <just/avbase/StreamType.h>
#include <just/avbase/StreamInfo.h>
#include <just/avbase/Sample.h>

namespace just
{
    namespace avcodec
    {

        using just::avbase::StreamType;
        using just::avbase::StreamSubType;
        using just::avbase::StreamFormatType;
        using just::avbase::VideoInfo;
        using just::avbase::AudioInfo;
        using just::avbase::StreamInfoBase;
        using just::avbase::StreamInfo;
        using just::avbase::Sample;

        class Codec;
        class Debuger;
        class Splitter;
        class Assembler;

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_BASE_H_
