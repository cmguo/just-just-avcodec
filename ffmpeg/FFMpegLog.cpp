// FFMpegLog.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/ffmpeg/FFMpegLog.h"

#include <framework/logger/Logger.h>
#include <framework/logger/VargRecord.h>

extern "C"
{
#include <libavutil/log.h>
}

namespace ppbox
{
    namespace avcodec
    {

        FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.avcodec.FFMpeg", framework::logger::Debug)

        static void av_log_callback(void* ptr, int level, const char* fmt, va_list vl)
        {
            LOG_VARG(level / 8, (fmt, vl));
        }

        void ffmpeg_log_setup()
        {
            av_log_set_callback(av_log_callback);
        }

    } // namespace avcodec
} // namespace ppbox
