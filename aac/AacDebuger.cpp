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
            return true;
        }

        bool AacDebuger::debug(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
