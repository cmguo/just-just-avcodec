// AudioCapture.h

#ifndef _PPBOX_AVCODEC_AUDIO_CAPTURE_H_
#define _PPBOX_AVCODEC_AUDIO_CAPTURE_H_

#include "ppbox/avcodec/Capture.h"

namespace ppbox
{
    namespace avcodec
    {

        class AudioCapture
            : public Capture
        {
        public:
            AudioCapture();

            virtual ~AudioCapture();

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

#endif // _PPBOX_AVCODEC_AUDIO_CAPTURE_H_
