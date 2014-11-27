// AvcEnum.h

#ifndef _JUST_AVCODEC_AVC_AVC_ENUM_H_
#define _JUST_AVCODEC_AVC_AVC_ENUM_H_

namespace just
{
    namespace avcodec
    {

        struct AvcSliceType
        {
            enum Enum
            {
                P = 0, 
                B, 
                I,
                SP,
                SI,
                P2 = 5,
                B2,
                I2,
                SP2,
                SI2
            };

            static char const * const type_str[];
        };

        struct AvcNaluType
        {
            enum Enum
            {
                undefine = 0,
                UNIDR,
                DATABLOCK_A,
                DATABLOCK_B,
                DATABLOCK_C,
                IDR,
                SEI,
                SPS,
                PPS,
                AccessUnitDelimiter,
                EndOfSeq,
                EndOfStream,
                FillerData,
                SPSExtension,
                Other,
            };

            static bool is_access_end(
                boost::uint8_t);

            static bool is_seq_aud(
                boost::uint8_t);

            static char const * const type_str[];
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_ENUM_H_
