// AacDebuger.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/aac/AacDebuger.h"
#include "just/avcodec/aac/AacConfig.h"
#include "just/avcodec/aac/AacConfigHelper.h"

#include <just/avbase/stream/BitsOStream.h>
#include <just/avbase/stream/BitsIStream.h>
#include <just/avbase/stream/BitsBuffer.h>
#include <just/avbase/stream/FormatBuffer.h>
using namespace just::avbase;

#include <util/archive/ArchiveBuffer.h>
#include <util/buffers/CycleBuffers.h>

namespace just
{
    namespace avcodec
    {

        bool AacDebuger::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            Debuger::reset(info, ec);
            AacConfigHelper const & config = *(AacConfigHelper const *)info.context;
            config.to_adts_data(0, adts_header_);
            return true;
        }

        bool AacDebuger::debug(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            boost::uint16_t len = (boost::uint16_t)sample.size + 7;
            // modify length 30th ~ 43th bits
            adts_header_[3] = (adts_header_[3] & 0xfC) | boost::uint8_t(len >> 11);
            adts_header_[4] = boost::uint8_t(len >> 3);
            adts_header_[5] = boost::uint8_t(len << 5) | (adts_header_[5] & 0x1f);
            dump(boost::asio::buffer(adts_header_));
            dump(sample.data);
            return true;
        }

    } // namespace avcodec
} // namespace just
