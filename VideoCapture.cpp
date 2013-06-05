// VideoCapture.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/VideoCapture.h"
#include "ppbox/avcodec/CodecType.h"

#include <framework/string/Parse.h>
using namespace framework::string;

namespace ppbox
{
    namespace avcodec
    {

        VideoCapture::VideoCapture()
        {
            info_.type = ppbox::avbase::StreamType::VIDE;
        }

        VideoCapture::~VideoCapture()
        {
        }

        bool VideoCapture::open(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            std::map<std::string, std::string>::const_iterator iter = config.begin();
            for (; iter != config.end(); ++iter) {
                std::string const & key = iter->first;
                std::string const & value = iter->second;
                if (key == "type") {
                    info_.sub_type = StreamType::from_string(value);
                } else if (key == "width") {
                    parse2(value, info_.video_format.width);
                } else if (key == "height") {
                    parse2(value, info_.video_format.height);
                } else if (key == "frame_rate") {
                    parse2(value, info_.video_format.frame_rate);
                    config_.frame_rate_num = info_.video_format.frame_rate;
                    config_.frame_rate_den = 1;
                }
            }
            return true;
        }

        bool VideoCapture::get(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            info = info_;
            return true;
        }

        bool VideoCapture::close(
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
