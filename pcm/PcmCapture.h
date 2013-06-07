// PcmCapture.h

#ifndef _PPBOX_AVCODEC_PCM_PCM_CAPTURE_H_
#define _PPBOX_AVCODEC_PCM_PCM_CAPTURE_H_

#include "ppbox/avcodec/csp/ColorSpace.h"
#include "ppbox/avcodec/AudioCapture.h"

namespace ppbox
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

        PPBOX_REGISTER_CAPTURE("pcm", PcmCapture);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_PCM_PCM_CAPTURE_H_
