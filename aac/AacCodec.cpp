// AacType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacCodec.h"
#include "ppbox/avcodec/aac/AacFormatType.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/aac/AacRawAssembler.h"
#include "ppbox/avcodec/aac/AacAdtsAssembler.h"
#include "ppbox/avcodec/aac/AacRawSplitter.h"
#include "ppbox/avcodec/aac/AacAdtsSplitter.h"
#include "ppbox/avcodec/aac/AacDebuger.h"
#include "ppbox/avcodec/aac/AacEncoder.h"

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
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            AacConfigHelper config;
            if (info.format_type == AacFormatType::raw) {
                config.from_data(info.format_data);
            } else if (info.format_type == AacFormatType::adts) {
                config.from_adts_data(info.format_data);
                config.to_adts_data(0, info.format_data);
            } else if (info.format_data.empty()) {
            } else {
                if (info.format_data.size() > 3 
                    && info.format_data[0] == 0xff
                    && (info.format_data[1] & 0xf0) == 0xf0) {
                        info.format_type = AacFormatType::adts;
                        config.from_adts_data(info.format_data);
                        config.to_adts_data(0, info.format_data);
                } else {
                    info.format_type = AacFormatType::raw;
                    config.from_data(info.format_data);
                }
            }
            if (!config.ready()) {
                ec = framework::system::logic_error::item_not_exist;
                return false;
            }
            config.get_format(info.audio_format);
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
