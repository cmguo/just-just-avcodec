// PcmCapture.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/pcm/PcmCapture.h"

namespace ppbox
{
    namespace avcodec
    {

        PcmCapture::PcmCapture()
            : index_(0)
        {
        }

        PcmCapture::~PcmCapture()
        {
        }

        bool PcmCapture::open(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            AudioCapture::open(config, ec);
            config_.max_frame_size = (info_.audio_format.sample_size / 8) 
                * info_.audio_format.channel_count 
                * config_.frame_rate_den;
            buffer_.resize(config_.max_frame_size);
            info_.format_data.assign((boost::uint8_t *)&config_, (boost::uint8_t *)(&config_ + 1));
            index_ = 0;
            return true;
        }

        bool PcmCapture::get(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            memset(&buffer_.at(0), index_, buffer_.size());
            sample.size = buffer_.size();
            sample.data.push_back(boost::asio::buffer(buffer_));
            ++index_;
            return true;
        }

        bool PcmCapture::close(
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace ppbox