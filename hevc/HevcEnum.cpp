// HevcType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/hevc/HevcEnum.h"
#include "ppbox/avcodec/hevc/HevcNaluHeader.h"

namespace ppbox
{
    namespace avcodec
    {

        bool HevcNaluType::is_access_end(
            boost::uint8_t byte)
        {
            HevcNaluHeader const nalu_header(byte, 0);
            return nalu_header.nal_unit_type <= HevcNaluType::TRAIL_R;
        }

        bool HevcNaluType::is_seq_aud(
            boost::uint8_t byte)
        {
            HevcNaluHeader const nalu_header(byte, 0);
            return nalu_header.nal_unit_type >= HevcNaluType::VPS_NUT
                && nalu_header.nal_unit_type <= HevcNaluType::AUD_NUT;
        }

        char const * const HevcNaluType::type_str[] = {
            "TRAIL_N",
            "TRAIL_R",
            "TSA_N",
            "TSA_R",
            "STSA_N",
            "STSA_R",
            "RADL_N",
            "RADL_R",
            "RASL_N",
            "RASL_R",
            "RSV_VCL_N10",
            "RSV_VCL_R11",
            "RSV_VCL_N12",
            "RSV_VCL_R13",
            "RSV_VCL_N14",
            "RSV_VCL_R15",
            "BLA_W_LP",
            "BLA_W_RADL",
            "BLA_N_LP",
            "IDR_W_RADL",
            "IDR_N_LP",
            "CRA_NUT",
            "RSV_IRAP_VCL22",
            "RSV_IRAP_VCL24",
            "RSV_VCL24",
            "RSV_VCL25",
            "RSV_VCL26",
            "RSV_VCL27",
            "RSV_VCL28",
            "RSV_VCL29",
            "RSV_VCL30",
            "RSV_VCL31",
            "VPS_NUT", 
            "SPS_NUT", 
            "PPS_NUT", 
            "AUD_NUT",
            "EOS_NUT",
            "EOB_NUT",
            "FD_NUT",
            "PREFIX_SEI_NUT",
            "SUFFIX_SEI_NUT",
            "RSV_NVCL41",
            "RSV_NVCL42",
            "RSV_NVCL43",
            "RSV_NVCL44",
            "RSV_NVCL45",
            "RSV_NVCL46",
            "RSV_NVCL47",
            "UNSPEC48",
        };

    } // namespace avcodec
} // namespace ppbox
