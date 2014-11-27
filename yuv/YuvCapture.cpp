// YuvCapture.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/yuv/YuvCapture.h"
#include "just/avcodec/VideoType.h"

namespace just
{
    namespace avcodec
    {

        YuvCapture::YuvCapture()
            : index_(0)
        {
            info_.sub_type = VideoType::I420;
        }

        YuvCapture::~YuvCapture()
        {
        }

        bool YuvCapture::open(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            VideoCapture::open(config, ec);
            ColorSpace::picture_size(info_, picture_, ec);
            config_.max_frame_size = picture_.total_size;
            buffer_.resize(config_.max_frame_size);
            info_.format_data.assign((boost::uint8_t *)&config_, (boost::uint8_t *)(&config_ + 1));
            index_ = 0;
            return true;
        }

        bool YuvCapture::get(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            memset(&buffer_.at(0), index_, buffer_.size());
            sample.size = buffer_.size();
            sample.data.push_back(boost::asio::buffer(buffer_));
            ++index_;
            return true;
        }

        bool YuvCapture::close(
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace just
