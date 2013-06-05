// VideoCapture.h

#ifndef _PPBOX_AVCODEC_VIDEO_CAPTURE_H_
#define _PPBOX_AVCODEC_VIDEO_CAPTURE_H_

#include "ppbox/avcodec/Capture.h"

namespace ppbox
{
    namespace avcodec
    {

        class VideoCapture
            : public Capture
        {
        public:
            VideoCapture();

            virtual ~VideoCapture();

        public:
            virtual bool open(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec);

            virtual bool get(
                StreamInfo & info, 
                boost::system::error_code & ec);

            virtual bool close(
                boost::system::error_code & ec);

        protected:
            StreamInfo info_;
            CaptureConfig config_;
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_YUV_YUV_CAPTURE_H_
