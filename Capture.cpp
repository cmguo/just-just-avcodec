// Capture.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Capture.h"
#include "ppbox/avcodec/Error.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/yuv/YuvCapture.h"
#include "ppbox/avcodec/pcm/PcmCapture.h"

namespace ppbox
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
} // namespace ppbox
