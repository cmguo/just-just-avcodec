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
            if (!info.format_data.empty()) {
                config.from_data(info.format_data);
            } else if (info.sub_type == AudioSubType::MP1A 
                || info.sub_type == AudioSubType::MP2A) {
                    config.set_version(info.sub_type == AudioSubType::MP1A
                        ? MpaConfigHelper::v1
                        : MpaConfigHelper::v2);
                    if (info.audio_format.sample_rate > 0) {
                        config.set_format(info.audio_format);
                        return true;
                    }
            }
            if (!config.ready()) {
                ec = framework::system::logic_error::item_not_exist;
                return false;
            }
            if (config.get_layer() == MpaConfigHelper::l3)
                info.sub_type = AudioSubType::MP3;
            else if (config.get_layer() == MpaConfigHelper::l2)
                info.sub_type = AudioSubType::MP2;
            config.get_format(info.audio_format);
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
