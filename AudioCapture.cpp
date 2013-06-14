// AudioCapture.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/AudioCapture.h"
#include "ppbox/avcodec/CodecType.h"

#include <framework/string/Parse.h>
using namespace framework::string;

namespace ppbox
{
    namespace avcodec
    {

        AudioCapture::AudioCapture()
        {
            info_.type = ppbox::avbase::StreamType::AUDI;
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
                    info_.sub_type = StreamType::from_string(value);
                } else if (key == "channel_count") {
                    parse2(value, info_.audio_format.channel_count);
                } else if (key == "sample_size") {
                    parse2(value, info_.audio_format.sample_size);
                } else if (key == "sample_rate") {
                    parse2(value, info_.audio_format.sample_rate);
                } else if (key == "sample_per_frame") {
                    parse2(value, info_.audio_format.sample_per_frame);
                }
                config_.frame_rate_num = info_.audio_format.sample_rate;
                config_.frame_rate_den = info_.audio_format.sample_per_frame;
            }
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
} // namespace ppbox
