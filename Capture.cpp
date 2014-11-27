// Capture.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/Capture.h"
#include "just/avcodec/Error.h"

#include <util/tools/ClassRegister.h>
#include "just/avcodec/yuv/YuvCapture.h"
#include "just/avcodec/pcm/PcmCapture.h"

namespace just
{
    namespace avcodec
    {

        CaptureConfig::CaptureConfig()
            : max_frame_size(0)
            , frame_rate_num(0)
            , frame_rate_den(0)
        {
        }

        Capture::Capture()
        {
        }

        Capture::~Capture()
        {
        }

        boost::system::error_code CaptureTraits::error_not_found()
        {
            return error::capture_not_support;
        }

    } // namespace avcodec
} // namespace just
