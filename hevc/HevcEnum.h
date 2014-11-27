// HevcEnum.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_ENUM_H_
#define _JUST_HEVCODEC_HEVC_HEVC_ENUM_H_

namespace just
{
    namespace avcodec
    {

        struct HevcNaluType
        {
            enum Enum
            {
                TRAIL_N = 0,
                TRAIL_R,
                TSA_N,
                TSA_R,
                STSA_N,
                STSA_R,
                RADL_N,
                RADL_R,
                RASL_N,
                RASL_R,
                RSV_VCL_N10,
                RSV_VCL_R11,
                RSV_VCL_N12,
                RSV_VCL_R13,
                RSV_VCL_N14,
                RSV_VCL_R15,
                BLA_W_LP,
                BLA_W_RADL,
                BLA_N_LP,
                IDR_W_RADL = 19,
                IDR_N_LP = 20,
                CRA_NUT = 21,
                RSV_IRAP_VCL22,
                RSV_IRAP_VCL23,
                RSV_VCL24,
                RSV_VCL25,
                RSV_VCL26,
                RSV_VCL27,
                RSV_VCL28,
                RSV_VCL29,
                RSV_VCL30,
                RSV_VCL31,
                VPS_NUT = 32, 
                SPS_NUT = 33, 
                PPS_NUT = 34, 
                AUD_NUT = 35,
                EOS_NUT = 36,
                EOB_NUT = 37,
                FD_NUT = 38,
                PREFIX_SEI_NUT = 39,
                SUFFIX_SEI_NUT = 40,
                RSV_NVCL41,
                RSV_NVCL42,
                RSV_NVCL43,
                RSV_NVCL44,
                RSV_NVCL45,
                RSV_NVCL46,
                RSV_NVCL47,
                UNSPEC48 = 48,
            };

            static bool is_access_end(
                boost::uint8_t);

            static bool is_seq_aud(
                boost::uint8_t);

            static char const * const type_str[];
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_ENUM_H_
