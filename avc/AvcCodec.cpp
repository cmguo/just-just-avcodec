// AvcCodec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcCodec.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcCodec::AvcCodec()
        {
        }

        bool AvcCodec::finish_stream_info(
            StreamInfo & info)
        {
            AvcConfigHelper config;
            if (info.format_type == AvcFormatType::packet) {
                config.from_data(info.format_data);
            } else {
                config.from_es_data(info.format_data);
            }
            if (!config.ready())
                return false;
            config.get_format(info.video_format);
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
