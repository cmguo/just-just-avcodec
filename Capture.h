// Capture.h

#ifndef _JUST_AVCODEC_CAPTURE_H_
#define _JUST_AVCODEC_CAPTURE_H_

#include "just/avcodec/Base.h"

#include <util/tools/ClassFactory.h>

namespace just
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
        {
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

        struct CaptureTraits
            : util::tools::ClassFactoryTraits
        {
            typedef std::string key_type;
            typedef Capture * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        typedef util::tools::ClassFactory<CaptureTraits> CaptureFactory;

    } // namespace avcodec
} // namespace just

#define JUST_REGISTER_CAPTURE(key, cls) UTIL_REGISTER_CLASS(just::avcodec::CaptureFactory, key, cls)

#endif // _JUST_AVCODEC_CAPTURE_H_
