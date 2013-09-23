// Capture.h

#ifndef _PPBOX_AVCODEC_CAPTURE_H_
#define _PPBOX_AVCODEC_CAPTURE_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        struct CaptureConfig
        {
            CaptureConfig();

            boost::uint32_t max_frame_size;
            boost::uint32_t frame_rate_num; // ·Ö×Ó
            boost::uint32_t frame_rate_den; // ·ÖÄ¸
        };

        class Capture
            : public util::tools::ClassFactory<
                Capture, 
                std::string, 
                Capture *()
            >
        {
        public:
            static boost::system::error_code error_not_found();

        public:
            Capture();

            virtual ~Capture();

        public:
            virtual bool open(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec) = 0;

            virtual bool get(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool get(
                Sample & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool free(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                assert(sample.context == NULL);
                return true;
            }

            virtual bool close(
                boost::system::error_code & ec) = 0;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_CAPTURE(key, cls) UTIL_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVCODEC_CAPTURE_H_
