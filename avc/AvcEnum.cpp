// AvcType.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/avc/AvcType.h"
#include "just/avcodec/avc/AvcEnum.h"

namespace just
{
    namespace avcodec
    {

        char const * const AvcSliceType::type_str[] = {
            "P", "B", "I", "SP", "SI", 
            "P2", "B2", "I2", "SP2", "SI2", 
        };

        bool AvcNaluType::is_access_end(
            boost::uint8_t byte)
        {
            AvcNaluHeader const nalu_header(byte);
            return nalu_header.nal_unit_type <= AvcNaluType::IDR;
        }

        bool AvcNaluType::is_seq_aud(
            boost::uint8_t byte)
        {
            AvcNaluHeader const nalu_header(byte);
            return nalu_header.nal_unit_type >= AvcNaluType::SPS
                && nalu_header.nal_unit_type <= AvcNaluType::AccessUnitDelimiter;
        }

        char const * const AvcNaluType::type_str[] = {
           "Unspecified", "non-IDR", "Slice A", "Slice B", "Slice C", 
           "IDR", "SEI", "SPS", "PPS", "AUD", "End Sequence", "End Stream", "Filler", 
        };

    } // namespace avcodec
} // namespace just
