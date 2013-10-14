// AacAdtsAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacAdtsAssembler.h"
#include "ppbox/avcodec/aac/AacConfigHelper.h"

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
            AacConfigHelper const & config = *(AacConfigHelper const *)info.context;
            config.to_adts_data(0, adts_header_);
            info.format_type = AacFormatType::adts;
            return true;
        }

        bool AacAdtsAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            boost::uint16_t len = (boost::uint16_t)sample.size + 7;
            // modify length 30th ~ 43th bits
            adts_header_[3] = (adts_header_[3] & 0xfC) | boost::uint8_t(len >> 11);
            adts_header_[4] = boost::uint8_t(len >> 3);
            adts_header_[5] = boost::uint8_t(len << 5) | (adts_header_[5] & 0x1f);
            sample.data.push_front(boost::asio::buffer(adts_header_));
            sample.size += adts_header_.size();

            return true;
        }

    } // namespace avcodec
} // namespace ppbox
