// Base.h

#ifndef _PPBOX_AVCODEC_BASE_H_
#define _PPBOX_AVCODEC_BASE_H_

#include <ppbox/avbase/StreamType.h>
#include <ppbox/avbase/StreamInfo.h>
#include <ppbox/avbase/Sample.h>

namespace ppbox
{
    namespace avcodec
    {

        using ppbox::avbase::StreamType;
        using ppbox::avbase::VideoInfo;
        using ppbox::avbase::AudioInfo;
        using ppbox::avbase::StreamInfoBase;
        using ppbox::avbase::StreamInfo;
        using ppbox::avbase::Sample;

        class Codec;
        class Debuger;
        class Splitter;
        class Assembler;

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_BASE_H_
