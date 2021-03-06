// VideoCapture.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/VideoCapture.h"
#include "just/avcodec/VideoType.h"

#include <framework/string/Parse.h>
#include <framework/string/Slice.h>
using namespace framework::string;

#include <iterator>

namespace just
{
    namespace avcodec
    {

        VideoCapture::VideoCapture()
        {
            info_.type = just::avbase::StreamType::VIDE;
            info_.video_format.width = 640;
            info_.video_format.height = 480;
            info_.video_format.frame_rate_num = 20;
            info_.video_format.frame_rate_den = 1;
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
                    info_.sub_type = just::avbase::FourCC::from_string(value);
                } else if (key == "width") {
                    parse2(value, info_.video_format.width);
                } else if (key == "height") {
                    parse2(value, info_.video_format.height);
                } else if (key == "frame_rate") {
                    std::vector<boost::uint32_t> vec;
                    slice<boost::uint32_t>(value, std::back_inserter(vec), "/");
                    if (vec.size() == 1) {
                        info_.video_format.frame_rate(vec[0]);
                    } else if (vec.size() == 2) {
                        info_.video_format.frame_rate(vec[0], vec[1]);
                    }
                }
            }
            config_.frame_rate_num = info_.video_format.frame_rate_num;
            config_.frame_rate_den = info_.video_format.frame_rate_den;
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
} // namespace just
