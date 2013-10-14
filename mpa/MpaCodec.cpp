// MpaType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/mpa/MpaCodec.h"

namespace ppbox
{
    namespace avcodec
    {

        MpaCodec::MpaCodec()
        {
        }

        MpaCodec::MpaCodec(
            boost::uint32_t format, 
            std::vector<boost::uint8_t> const & config)
        {
            assert(format == StreamFormatType::none);
            config_helper_.from_data(config);
        }

        bool MpaCodec::finish_stream_info(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            assert(info.format_type == StreamFormatType::none);
            MpaConfigHelper config;
            if (!info.format_data.empty())
                config.from_data(info.format_data);
            config.get_format(info.audio_format);
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
