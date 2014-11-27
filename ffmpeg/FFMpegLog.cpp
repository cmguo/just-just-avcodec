// FFMpegLog.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/ffmpeg/FFMpegLog.h"

#include <framework/logger/Logger.h>
#include <framework/logger/VargRecord.h>

extern "C"
{
#define UINT64_C(c)   c ## ULL
#include <libavutil/log.h>
}

namespace just
{
    namespace avcodec
    {

        FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.avcodec.FFMpeg", framework::logger::Debug)

        static void av_log_callback(void* ptr, int level, const char* fmt, va_list vl)
        {
            LOG_VARG(level / 8, (fmt, vl));
        }

        void ffmpeg_log_setup()
        {
            av_log_set_callback(av_log_callback);
        }

    } // namespace avcodec
} // namespace just
