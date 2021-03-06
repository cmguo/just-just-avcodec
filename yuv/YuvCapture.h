// YuvCapture.h

#ifndef _JUST_AVCODEC_YUV_YUV_CAPTURE_H_
#define _JUST_AVCODEC_YUV_YUV_CAPTURE_H_

#include "just/avcodec/csp/ColorSpace.h"
#include "just/avcodec/VideoCapture.h"

namespace just
{
    namespace avcodec
    {

        class YuvCapture
            : public VideoCapture
        {
        public:
            YuvCapture();

            virtual ~YuvCapture();

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
            ColorSpace::PictureSize picture_;
            boost::uint8_t index_;
            std::vector<boost::uint8_t> buffer_;
        };

        JUST_REGISTER_CAPTURE("yuv", YuvCapture);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_YUV_YUV_CAPTURE_H_
