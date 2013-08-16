// Capture.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Capture.h"
#include "ppbox/avcodec/Error.h"

#include <ppbox/common/ClassRegister.h>
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

        boost::system::error_code Capture::error_not_found()
        {
            return error::capture_not_support;
        }

        Capture::Capture()
        {
        }

        Capture::~Capture()
        {
        }

    } // namespace avcodec
} // namespace ppbox
