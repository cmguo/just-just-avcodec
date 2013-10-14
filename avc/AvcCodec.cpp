// AvcCodec.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcCodec.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcPacketAssembler.h"
#include "ppbox/avcodec/avc/AvcByteStreamAssembler.h"
#include "ppbox/avcodec/avc/AvcPacketSplitter.h"
#include "ppbox/avcodec/avc/AvcByteStreamSplitter.h"
#include "ppbox/avcodec/avc/AvcDebuger.h"
#include "ppbox/avcodec/avc/AvcEncoder.h"

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
            } else if (info.format_type == AvcFormatType::byte_stream) {
                config.from_es_data(info.format_data);
                config.to_es_data(info.format_data);
            } else if (info.format_data.empty()) {
            } else {
                if (info.format_data.at(0) == 1) {
                    info.format_type = AvcFormatType::packet;
                    config.from_data(info.format_data);
                } else {
                    info.format_type = AvcFormatType::byte_stream;
                    config.from_es_data(info.format_data);
                    config.to_es_data(info.format_data);
                }
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
