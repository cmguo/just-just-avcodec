// Encoder.h

#ifndef _PPBOX_AVCODEC_ENCODER_H_
#define _PPBOX_AVCODEC_ENCODER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Encoder
            : public ppbox::common::ClassFactory<
                Encoder, 
                boost::uint32_t, 
                Encoder *()
            >
        {
        public:
            Encoder()
            {
            }

            virtual ~Encoder()
            {
            }

        public:
            virtual bool open(
                StreamInfo const & input_format, 
                std::map<std::string, std::string> const & config, 
                StreamInfo & output_format, 
                boost::system::error_code & ec) = 0;

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool close(
                boost::system::error_code & ec) = 0;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_ENCODER(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVCODEC_ENCODER_H_
