// AacType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacCodec.h"

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
            if (format == FormatType::audio_raw) {
                config_helper_.from_data(config);
            } else {
                config_helper_.from_adts_data(config);
            }
        }

    } // namespace avcodec
} // namespace ppbox
