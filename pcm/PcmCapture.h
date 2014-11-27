// PcmCapture.h

#ifndef _JUST_AVCODEC_PCM_PCM_CAPTURE_H_
#define _JUST_AVCODEC_PCM_PCM_CAPTURE_H_

#include "just/avcodec/csp/ColorSpace.h"
#include "just/avcodec/AudioCapture.h"

namespace just
{
    namespace avcodec
    {

        class PcmCapture
            : public AudioCapture
        {
        public:
            PcmCapture();

            virtual ~PcmCapture();

        public:
            virtual bool open(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec);

            virtual bool get(
                Sample & sample, 
                boost::system::error_code & ec);

            virtual bool close(
                boost::system::error_code & ec);

        private:
            boost::uint8_t index_;
            std::vector<boost::uint8_t> buffer_;
        };

        JUST_REGISTER_CAPTURE("pcm", PcmCapture);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_PCM_PCM_CAPTURE_H_
