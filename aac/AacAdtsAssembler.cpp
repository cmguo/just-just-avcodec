// AacAdtsAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacAdtsAssembler.h"

namespace ppbox
{
    namespace avcodec
    {

        AacAdtsAssembler::AacAdtsAssembler()
        {
        }

        AacAdtsAssembler::~AacAdtsAssembler()
        {
        }

        bool AacAdtsAssembler::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_data(info.format_data);
            return true;
        }

        bool AacAdtsAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            config_.to_adts_data(sample.size, adts_header_);
            sample.data.push_front(boost::asio::buffer(adts_header_));
            sample.size += adts_header_.size();

            return true;
        }

    } // namespace avcodec
} // namespace ppbox
