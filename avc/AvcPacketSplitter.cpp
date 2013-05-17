// AvcPacketSplitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcPacketSplitter.h"
#include "ppbox/avcodec/avc/AvcConfig.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcPacketSplitter::AvcPacketSplitter()
        {
        }

        AvcPacketSplitter::~AvcPacketSplitter()
        {
        }

        bool AvcPacketSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_data(info.format_data);
            helper_.nalu_length_size((boost::uint8_t)config_.data().lengthSizeMinusOne + 1);
            info.context = &config_;
            return true;
        }

        bool AvcPacketSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            sample.context = &helper_;
            return helper_.from_packet(sample.size, sample.data);
        }

    } // namespace avcodec
} // namespace ppbox
