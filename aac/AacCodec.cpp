// AacType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacCodec.h"
#include "ppbox/avcodec/aac/AacFormatType.h"

namespace ppbox
{
    namespace avcodec
    {

        AacCodec::AacCodec()
        {
        }

        AacCodec::AacCodec(
            boost::uint32_t format, 
            std::vector<boost::uint8_t> const & config)
        {
            if (format == AacFormatType::raw) {
                config_helper_.from_data(config);
            } else {
                config_helper_.from_adts_data(config);
            }
        }

        bool AacCodec::finish_stream_info(
            StreamInfo & info)
        {
            AacConfigHelper config;
            if (info.format_type == AacFormatType::raw) {
                config.from_data(info.format_data);
            } else {
                config.from_adts_data(info.format_data);
            }
            config.get_format(info.audio_format);
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
