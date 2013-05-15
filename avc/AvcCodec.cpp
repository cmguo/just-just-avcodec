// AvcCodec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcCodec.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcCodec::AvcCodec()
        {
        }

        AvcCodec::AvcCodec(
            boost::uint32_t format, 
            std::vector<boost::uint8_t> const & config)
        {
            if (format == FormatType::video_avc_packet) {
                config_helper_.from_data(config);
            } else {
                config_helper_.from_es_data(config);
            }
        }

    } // namespace avcodec
} // namespace ppbox
