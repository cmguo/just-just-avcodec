// HevcNaluHeader.h

#ifndef _JUST_AVCODEC_HEVC_HEVC_NALU_HEADER_H_
#define _JUST_AVCODEC_HEVC_HEVC_NALU_HEADER_H_

#include "just/avcodec/BitsType.h"

namespace just
{
    namespace avcodec
    {

        struct HevcNaluHeader
        {
            U<1> forbidden_zero_bit;
            U<6> nal_unit_type;
            U<6> nuh_layer_id;
            U<3> nuh_temporal_id_plus1;

            HevcNaluHeader(
                U<5> const & nal_unit_type = 0)
                : nal_unit_type(nal_unit_type)
            {
            }

            HevcNaluHeader(
                boost::uint8_t b1, 
                boost::uint8_t b2)
            {
                nal_unit_type = (b1 >> 1) & 0x3f;
                nuh_layer_id = ((b1 & 0x01) << 5) | ((b2 >> 3) & 0x1f);
                nuh_temporal_id_plus1 = b2 & 0x07;
            }

            HevcNaluHeader(
                boost::uint16_t w)
            {
                nal_unit_type = (w >> 9) & 0x3f;
                nuh_layer_id = (w >> 3) & 0x3f;
                nuh_temporal_id_plus1 = w & 0x07;
            }

            boost::uint8_t byte1() const
            {
                return ((boost::uint8_t)nal_unit_type << 1) 
                    | ((boost::uint8_t)nuh_layer_id >> 5);
            }

            boost::uint8_t byte2() const
            {
                return ((boost::uint8_t)nuh_layer_id << 3) 
                    | (boost::uint8_t)nuh_temporal_id_plus1;
            }

            boost::uint16_t word() const
            {
                return ((boost::uint16_t)nal_unit_type << 9) 
                    | ((boost::uint16_t)nuh_layer_id << 3) 
                    | (boost::uint16_t)nuh_temporal_id_plus1;
            }

            template <
                typename Archive
            >
            void serialize(
                Archive & ar)
            {
                ar & forbidden_zero_bit
                    & nal_unit_type
                    & nuh_layer_id
                    & nuh_temporal_id_plus1;
            }

        private:
            HevcNaluHeader(
                boost::uint8_t b); // disable unexpecttly convert to boost::uint16_t
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_HEVC_HEVC_NALU_HEADER_H_
