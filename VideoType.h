// VideoType.h

#ifndef _PPBOX_AVCODEC_VIDEO_TYPE_H_
#define _PPBOX_AVCODEC_VIDEO_TYPE_H_

#include "ppbox/avcodec/Base.h"

namespace ppbox
{
    namespace avcodec
    {

        struct VideoType
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
                BGRA = MAKE_FOURC_TYPE('B', 'G', 'R', 'A'), // BGRA32
                // YUV                                      // Sampling Type    Bits
                //     9 bits
                NV11 = MAKE_FOURC_TYPE('N', 'V', '1', '1'), // 4:1:1    Planar  8 
                Y41P = MAKE_FOURC_TYPE('Y', '4', '1', 'P'), // 4:1:1    Packed  8 
                Y41T = MAKE_FOURC_TYPE('Y', '4', '1', 'T'), // 4:1:1    Packed  8 
                //    12 bits
                I420 = MAKE_FOURC_TYPE('I', '4', '2', '0'), // 4:2:0    Planar  8 Y4 V1 U1
                YV12 = MAKE_FOURC_TYPE('Y', 'V', '1', '2'), // 4:2:0    Planar  8 Y4 U1 V1
                NV12 = MAKE_FOURC_TYPE('N', 'V', '1', '2'), // 4:2:0    Planar  8 with one y plane and one packed u+v
                NV21 = MAKE_FOURC_TYPE('N', 'V', '2', '1'), // 4:2:0    Planar  8 with one y plane and one packed v+u
                IYUV = MAKE_FOURC_TYPE('I', 'Y', 'U', 'Y'), // 4:2:0    Planar  8 
                UYVY = MAKE_FOURC_TYPE('U', 'Y', 'V', 'Y'), // 4:2:2    Packed  8 
                YUY2 = MAKE_FOURC_TYPE('Y', 'U', 'Y', '2'), // 4:2:2    Packed  8 
                Y42T = MAKE_FOURC_TYPE('Y', '4', '2', 'T'), // 4:2:2    Packed  8 
                //    16 bits
                I422 = MAKE_FOURC_TYPE('I', '4', '2', '2'), // 4:2:2    Planar  8 
                YV16 = MAKE_FOURC_TYPE('Y', 'V', '1', '6'), // 4:2:2    Planar  8 with one y plane and one packed u+v
                NV16 = MAKE_FOURC_TYPE('N', 'V', '1', '6'), // 4:2:2    Planar  8 with one y plane and one packed u+v
                //    24 bits
                I444 = MAKE_FOURC_TYPE('I', '4', '4', '4'), // 4:4:4    Planar  8
                YV24 = MAKE_FOURC_TYPE('Y', 'V', '2', '4'), // 4:4:4    Planar  8
                AI44 = MAKE_FOURC_TYPE('A', 'I', '4', '4'), // 4:4:4    Packed  Palettized 
                AYUV = MAKE_FOURC_TYPE('A', 'Y', 'U', 'V'), // 4:4:4    Packed  8 
                // Compress
                // MPEG
                MP2V = MAKE_FOURC_TYPE('M', 'P', '2', 'V'), // H.262
                MP4V = MAKE_FOURC_TYPE('M', 'P', '4', 'V'), 
                AVC  = MAKE_FOURC_TYPE('A', 'V', 'C', 0),   // H.264
                HEVC = MAKE_FOURC_TYPE('H', 'E', 'V', 'C'), // H.265
                // ITU-T
                H262 = MAKE_FOURC_TYPE('H', '2', '6', '2'), 
                H263 = MAKE_FOURC_TYPE('H', '2', '6', '3'), 
                H264 = MAKE_FOURC_TYPE('H', '2', '6', '4'), 
                H265 = MAKE_FOURC_TYPE('H', '2', '6', '5'), 
                // Microsoft
                MPG4 = MAKE_FOURC_TYPE('M', 'P', 'G', '4'), 
                MP41 = MAKE_FOURC_TYPE('M', 'P', '4', '1'), 
                MP42 = MAKE_FOURC_TYPE('M', 'P', '4', '2'), 
                MP43 = MAKE_FOURC_TYPE('M', 'P', '4', '3'), 
                WMV2 = MAKE_FOURC_TYPE('W', 'M', 'V', '2'), 
                WMV3 = MAKE_FOURC_TYPE('W', 'M', 'V', '3'), // Simple & Main Profile of VC-1, VC-1 = WMV3 + WMVA + WVC1
                // RealNetworks
                RV30 = MAKE_FOURC_TYPE('R', 'V', '3', '0'), 
                RV40 = MAKE_FOURC_TYPE('R', 'V', '4', '0'), 
                // Google/On2
                VP6  = MAKE_FOURC_TYPE('V', 'P', '6', 0),
                VP7  = MAKE_FOURC_TYPE('V', 'P', '7', 0),
                VP8  = MAKE_FOURC_TYPE('V', 'P', '8', 0),
                VP9  = MAKE_FOURC_TYPE('V', 'P', '9', 0),
            };
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_VIDEO_TYPE_H_
