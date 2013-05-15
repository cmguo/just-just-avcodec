// AvcEncoder.h

#ifndef _PPBOX_AVCODEC_ACV_AVC_ENCODER_H_
#define _PPBOX_AVCODEC_ACV_AVC_ENCODER_H_

#include "ppbox/avcodec/Encoder.h"
#include "ppbox/avcodec/Format.h"

namespace ppbox
{
    namespace avcodec
    {

        struct x264_impl;

        class AvcEncoder
            : public Encoder
        {
        public:
            AvcEncoder();

            virtual ~AvcEncoder();

        public:
            virtual bool open(
                StreamInfo const & input_format, 
                std::map<std::string, std::string> const & config, 
                StreamInfo & output_format, 
                boost::system::error_code & ec);

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec);

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec);

            virtual bool close(
                boost::system::error_code & ec);

        private:
            x264_impl * impl_;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_ENCODER(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVCODEC_ACV_AVC_ENCODER_H_
