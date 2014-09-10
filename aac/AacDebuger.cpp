// AacDebuger.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacDebuger.h"
#include "ppbox/avcodec/aac/AacConfig.h"
#include "ppbox/avcodec/aac/AacConfigHelper.h"

#include <ppbox/avbase/stream/BitsOStream.h>
#include <ppbox/avbase/stream/BitsIStream.h>
#include <ppbox/avbase/stream/BitsBuffer.h>
#include <ppbox/avbase/stream/FormatBuffer.h>
using namespace ppbox::avbase;

#include <util/archive/ArchiveBuffer.h>
#include <util/buffers/CycleBuffers.h>

namespace ppbox
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
} // namespace ppbox
