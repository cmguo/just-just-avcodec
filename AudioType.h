// AudioType.h

#ifndef _PPBOX_AVCODEC_AUDIO_TYPE_H_
#define _PPBOX_AVCODEC_AUDIO_TYPE_H_

#include "ppbox/avcodec/CodecType.h"

namespace ppbox
{
    namespace avcodec
    {

        struct AudioType
        {
            enum Enum
            {
                // MPEG
                MP1  = MAKE_FOURC_TYPE('M', 'P', '1', 0), // MPEG-1 or MPEG-2 Audio Layer I
                MP2  = MAKE_FOURC_TYPE('M', 'P', '2', 0), // MPEG-1 or MPEG-2 Audio Layer II
                MP3  = MAKE_FOURC_TYPE('M', 'P', '3', 0), // MPEG-1 or MPEG-2 Audio Layer III
                MP1A = MAKE_FOURC_TYPE('M', 'P', '1', 'A'), 
                MP2A = MAKE_FOURC_TYPE('M', 'P', '2', 'A'), 
                AAC  = MAKE_FOURC_TYPE('A', 'A', 'C', 0), 
                // ITU-T
                G711 = MAKE_FOURC_TYPE('G', '7', '1', '1'),
                G719 = MAKE_FOURC_TYPE('G', '7', '1', '9'),
                // Microsoft
                WMA2 = MAKE_FOURC_TYPE('W', 'M', 'A', '2'), 
                // Dolby
                AC3  = MAKE_FOURC_TYPE('A', 'C', '3', 0), 
                EAC3 = MAKE_FOURC_TYPE('E', 'A', 'C', '3'), 
                // RealNetworks
                COOK = MAKE_FOURC_TYPE('C', 'O', 'O', 'K'), 
                // Xiph.org
                FLAC = MAKE_FOURC_TYPE('F', 'L', 'A', 'C'), 
                VORB = MAKE_FOURC_TYPE('V', 'O', 'R', 'B'), // Ogg Vorbis
                // else
                DTS  = MAKE_FOURC_TYPE('D', 'T', 'S', 0), 
                AMR  = MAKE_FOURC_TYPE('A', 'M', 'R', 0),
                // Non-compress
                PCM  = MAKE_FOURC_TYPE('P', 'C', 'M', 0), 
                FLT  = MAKE_FOURC_TYPE('F', 'L', 'T', 0), 
            };
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AUDIO_TYPE_H_
