// MpaConfig.h

#ifndef _JUST_AVCODEC_MPA_MPA_CONFIG_H_
#define _JUST_AVCODEC_MPA_MPA_CONFIG_H_

#include "just/avcodec/BitsType.h"

namespace just
{
    namespace avcodec
    {

        // MPEG Audio Frame Header

        /*
        Sign Length(bits) Position(bits) Description 
        A       11      (31-21)         Frame sync (all bits set) 
        B       2       (20,19)         MPEG Audio version ID
        C       2       (18,17)         Layer description
        D       1       (16)            Protection bit 0 - Protected by CRC (16bit crc follows header) 1 - Not protected 
        E       4       (15,12)         Bitrate index
        F       2       (11,10)         Sampling rate frequency index (values are in Hz) bits MPEG1 MPEG2 MPEG2.5 
        G       1       (9)             Padding bit
        H       1       (8)             Private bit. It may be freely used for specific needs of an application, i.e. if it has to trigger some application specific events. 
        I       2       (7,6)           Channel Mode
        J       2       (5,4)           Mode extension (Only if Joint stereo) 
        K       1       (3)             Copyright
        L       1       (2)             Original
        M       2       (1,0)           Emphasis
         */

        struct MpaConfig
        {
            U<12> syncword;
            U<1> version; // 0: MPEG-4 part 3, 1: MPEG-2 part 7
            U<2> layer;
            U<1> protection_absent; // crc

            U<4> bitrate_index;
            U<2> frequency_index;
            U<1> padding_bit;
            U<1> private_bit;

            U<2> channel_mode;
            U<2> mode_extension;
            U<1> copyright;
            U<1> original;
            U<2> emphasis;

            //U<16> crc_check;

            MpaConfig()
                : syncword(0x0fff)
                , protection_absent(1)
            {
            }

            template <
                typename Archive
            >
            void serialize(
                Archive & ar)
            {
                ar & syncword
                    & version
                    & layer
                    & protection_absent
                    & bitrate_index
                    & frequency_index
                    & padding_bit
                    & private_bit
                    & channel_mode
                    & mode_extension
                    & copyright
                    & original
                    & emphasis
                    ;
            }
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_MPA_MPA_CONFIG_H_
