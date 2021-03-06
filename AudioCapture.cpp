// AudioCapture.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/AudioCapture.h"
#include "just/avcodec/AudioType.h"

#include <framework/string/Parse.h>
using namespace framework::string;

namespace just
{
    namespace avcodec
    {

        AudioCapture::AudioCapture()
        {
            info_.type = just::avbase::StreamType::AUDI;
            info_.audio_format.channel_count = 1;
            info_.audio_format.sample_size = 8;
            info_.audio_format.sample_rate = 22050;
            info_.audio_format.sample_per_frame = 1024;
        }

        AudioCapture::~AudioCapture()
        {
        }

        bool AudioCapture::open(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            std::map<std::string, std::string>::const_iterator iter = config.begin();
            for (; iter != config.end(); ++iter) {
                std::string const & key = iter->first;
                std::string const & value = iter->second;
                if (key == "type") {
                    info_.sub_type = just::avbase::FourCC::from_string(value);
                } else if (key == "channel_count") {
                    parse2(value, info_.audio_format.channel_count);
                } else if (key == "sample_size") {
                    parse2(value, info_.audio_format.sample_size);
                } else if (key == "sample_rate") {
                    parse2(value, info_.audio_format.sample_rate);
                } else if (key == "sample_per_frame") {
                    parse2(value, info_.audio_format.sample_per_frame);
                }
            }
            config_.frame_rate_num = info_.audio_format.sample_rate;
            config_.frame_rate_den = info_.audio_format.sample_per_frame;
            return true;
        }

        bool AudioCapture::get(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            info = info_;
            return true;
        }

        bool AudioCapture::close(
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace just
