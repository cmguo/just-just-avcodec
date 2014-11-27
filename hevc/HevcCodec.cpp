// HevcCodec.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/hevc/HevcCodec.h"
#include "just/avcodec/hevc/HevcFormatType.h"
#include "just/avcodec/hevc/HevcConfigHelper.h"

#include <util/tools/ClassRegister.h>
#include "just/avcodec/hevc/HevcPacketAssembler.h"
#include "just/avcodec/hevc/HevcByteStreamAssembler.h"
#include "just/avcodec/hevc/HevcPacketSplitter.h"
#include "just/avcodec/hevc/HevcByteStreamSplitter.h"
#include "just/avcodec/hevc/HevcDebuger.h"
//#include "just/avcodec/hevc/HevcEncoder.h"

namespace just
{
    namespace avcodec
    {

        HevcCodec::HevcCodec()
        {
        }

        bool HevcCodec::finish_stream_info(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            HevcConfigHelper config;
            if (info.format_type == HevcFormatType::packet) {
                config.from_data(info.format_data);
            } else if (info.format_type == HevcFormatType::byte_stream) {
                config.from_es_data(info.format_data);
                config.to_es_data(info.format_data);
            } else if (info.format_data.empty()) {
            } else {
                if (info.format_data.at(0) == 1) {
                    info.format_type = HevcFormatType::packet;
                    config.from_data(info.format_data);
                } else {
                    info.format_type = HevcFormatType::byte_stream;
                    config.from_es_data(info.format_data);
                    config.to_es_data(info.format_data);
                }
            }
            //if (!config.ready()) {
            //    ec = framework::system::logic_error::item_not_exist;
            //    return false;
            //}
            config.get_format(info.video_format);
            ec.clear();
            return true;
        }

    } // namespace avcodec
} // namespace just
