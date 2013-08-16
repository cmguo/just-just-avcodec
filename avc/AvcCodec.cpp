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
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            AvcConfigHelper config;
            if (info.format_type == AvcFormatType::packet) {
                config.from_data(info.format_data);
            } else {
                config.from_es_data(info.format_data);
            }
            if (!config.ready()) {
                ec = framework::system::logic_error::item_not_exist;
                return false;
            }
            config.get_format(info.video_format);
            ec.clear();
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
