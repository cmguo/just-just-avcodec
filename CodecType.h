// CodecType.h

#ifndef _PPBOX_AVCODEC_CODEC_TYPE_H_
#define _PPBOX_AVCODEC_CODEC_TYPE_H_

#include <ppbox/avbase/StreamType.h>

namespace ppbox
{
    namespace avcodec
    {

        using ppbox::avbase::StreamType;
        using ppbox::avbase::StreamSubType;
        using ppbox::avbase::StreamFormatType;

        struct VideoSubType
        {
            enum Enum
            {
                // RGB
                RGB8 = MAKE_FOURC_TYPE('R', 'G', 'B', '8'), // RGB
                RGB5 = MAKE_FOURC_TYPE('R', 'G', 'B', '5'), // RGB555
                RGB6 = MAKE_FOURC_TYPE('R', 'G', 'B', '6'), // RGB565
                RGBT = MAKE_FOURC_TYPE('R', 'G', 'B', 'T'), // RGB24
                RGBX = MAKE_FOURC_TYPE('R', 'G', 'B', 'X'), // RGB32
                BGR8 = MAKE_FOURC_TYPE('B', 'G', 'R', '8'), // BGR
                ARGB = MAKE_FOURC_TYPE('A', 'R', 'G', 'B'), // ARGB32
                // YUV                                      // Sampling Type    Bits
                I420 = MAKE_FOURC_TYPE('I', '4', '2', '0'), // 4:2:0    Planar  8 Y4 V1 U1
                YV12 = MAKE_FOURC_TYPE('Y', 'V', '1', '2'), // 4:2:0    Planar  8 Y4 U1 V1
                IYUV = MAKE_FOURC_TYPE('I', 'Y', 'U', 'Y'), // 4:2:0    Planar  8 
                NV12 = MAKE_FOURC_TYPE('N', 'V', '1', '2'), // 4:2:0    Planar  8 with one y plane and one packed u+v
                I422 = MAKE_FOURC_TYPE('I', '4', '2', '2'), // 4:2:2    Planar  8 
                YV16 = MAKE_FOURC_TYPE('Y', 'V', '1', '6'), // 4:2:2    Planar  8 with one y plane and one packed u+v
                NV11 = MAKE_FOURC_TYPE('N', 'V', '1', '1'), // 4:1:1    Planar  8 
                I444 = MAKE_FOURC_TYPE('I', '4', '4', '4'), // 4:4:4    Planar  8
                YV24 = MAKE_FOURC_TYPE('Y', 'V', '2', '4'), // 4:4:4    Planar  8
                AI44 = MAKE_FOURC_TYPE('A', 'I', '4', '4'), // 4:4:4    Packed  Palettized 
                AYUV = MAKE_FOURC_TYPE('A', 'Y', 'U', 'V'), // 4:4:4    Packed  8 
                UYVY = MAKE_FOURC_TYPE('U', 'Y', 'V', 'Y'), // 4:2:2    Packed  8 
                Y41P = MAKE_FOURC_TYPE('Y', '4', '1', 'P'), // 4:1:1    Packed  8 
                Y41T = MAKE_FOURC_TYPE('Y', '4', '1', 'T'), // 4:1:1    Packed  8 
                Y42T = MAKE_FOURC_TYPE('Y', '4', '2', 'T'), // 4:2:2    Packed  8 
                YUY2 = MAKE_FOURC_TYPE('Y', 'U', 'Y', '2'), // 4:2:2    Packed  8 
                // Compress
                AVC1 = MAKE_FOURC_TYPE('A', 'V', 'C', '1'), 
                WMV3 = MAKE_FOURC_TYPE('W', 'M', 'V', '3'), 
                MP4V = MAKE_FOURC_TYPE('M', 'P', '4', 'V'), 
            };
        };

        struct AudioSubType
        {
            enum Enum
            {
                MP4A = MAKE_FOURC_TYPE('M', 'P', '4', 'A'), 
                MP1A = MAKE_FOURC_TYPE('M', 'P', '1', 'A'), 
                WMA2 = MAKE_FOURC_TYPE('W', 'M', 'A', '2'), 
            };
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_CODEC_TYPE_H_
