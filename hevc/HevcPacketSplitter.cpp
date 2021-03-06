// HevcPacketSplitter.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/hevc/HevcPacketSplitter.h"
#include "just/avcodec/hevc/HevcConfig.h"

namespace just
{
    namespace avcodec
    {

        HevcPacketSplitter::HevcPacketSplitter()
        {
        }

        HevcPacketSplitter::~HevcPacketSplitter()
        {
        }

        bool HevcPacketSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_data(info.format_data);
            helper_.nalu_length_size((boost::uint8_t)config_.data().lengthSizeMinusOne + 1);
            info.context = &config_;
            return true;
        }

        bool HevcPacketSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            sample.context = &helper_;
            return helper_.from_packet(sample.size, sample.data);
        }

    } // namespace avcodec
} // namespace just
