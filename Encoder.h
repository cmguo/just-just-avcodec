// Encoder.h

#ifndef _PPBOX_AVCODEC_ENCODER_H_
#define _PPBOX_AVCODEC_ENCODER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Encoder
        {
        public:
            struct eos_t { };

            static eos_t eos()
            {
                return eos_t();
            }

        public:
            Encoder();

            virtual ~Encoder();

        public:
            virtual bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec) = 0;

            virtual bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec) = 0;

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool push(
                eos_t const & eos, 
                boost::system::error_code & ec) = 0;

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool refresh(
                boost::system::error_code & ec) = 0;

            virtual bool close(
                boost::system::error_code & ec) = 0;
        };

        struct EncoderTraits
            : util::tools::ClassFactoryTraits
        {
            typedef boost::uint32_t key_type;
            typedef Encoder * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        typedef util::tools::ClassFactory<EncoderTraits> EncoderFactory;

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_ENCODER(key, cls) UTIL_REGISTER_CLASS(ppbox::avcodec::EncoderFactory, key, cls)

#endif // _PPBOX_AVCODEC_ENCODER_H_
