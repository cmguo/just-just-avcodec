// Format.h

#ifndef _PPBOX_AVCODEC_FORMAT_H_
#define _PPBOX_AVCODEC_FORMAT_H_

#include <ppbox/avbase/Format.h>

namespace ppbox
{
    namespace avcodec
    {

        using ppbox::avbase::StreamType;

        struct VideoSubType
        {
            enum Enum
            {
                NONE = 0, 
                AVC1 = MAKE_FOURC_TYPE('A', 'V', 'C', '1'), 
                WMV3 = MAKE_FOURC_TYPE('W', 'M', 'V', '3'), 
                MP4V = MAKE_FOURC_TYPE('M', 'P', '4', 'V'), 
            };
        };

        struct AudioSubType
        {
            enum Enum
            {
                NONE = 0, 
                MP4A = MAKE_FOURC_TYPE('M', 'P', '4', 'A'), 
                MP1A = MAKE_FOURC_TYPE('M', 'P', '1', 'A'), 
                WMA2 = MAKE_FOURC_TYPE('W', 'M', 'A', '2'), 
            };
        };

        struct FormatType
        {
            enum Enum
            {
                none = 0, 
                video_avc_packet = 1, 
                video_avc_byte_stream = 2, 
                audio_raw = 1, 
                audio_adts = 2, 
            };
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_FORMAT_H_
