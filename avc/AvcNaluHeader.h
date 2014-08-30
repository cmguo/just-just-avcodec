// AvcNaluHeader.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_NALU_HEADER_H_
#define _PPBOX_AVCODEC_AVC_AVC_NALU_HEADER_H_

#include "ppbox/avcodec/BitsType.h"

namespace ppbox
{
    namespace avcodec
    {

        struct AvcNaluHeader
        {
            U<1> forbidden_zero_bit;
            U<2> nal_ref_idc;
            U<5> nal_unit_type;

            AvcNaluHeader(
                U<5> const & nal_unit_type = 0)
                : nal_unit_type(nal_unit_type)
            {
            }

            AvcNaluHeader(
                boost::uint8_t b)
            {
                nal_ref_idc = (b & 0x60) >> 5;
                nal_unit_type = b & 0x1f;
            }

            boost::uint8_t byte() const
            {
                return (boost::uint8_t)nal_ref_idc << 5 | (boost::uint8_t)nal_unit_type;
            }

            template <
                typename Archive
            >
            void serialize(
                Archive & ar)
            {
                ar & forbidden_zero_bit
                    & nal_ref_idc
                    & nal_unit_type;
            }
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_NALU_HEADER_H_
