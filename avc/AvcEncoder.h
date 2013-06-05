// AvcEncoder.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_ENCODER_H_
#define _PPBOX_AVCODEC_AVC_AVC_ENCODER_H_

#include "ppbox/avcodec/Encoder.h"
#include "ppbox/avcodec/CodecType.h"

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
            virtual bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec);

            virtual bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec);

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec);

            virtual bool push(
                eos_t const & eos, 
                boost::system::error_code & ec);

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec);

            virtual bool refresh(
                boost::system::error_code & ec);

            virtual bool close(
                boost::system::error_code & ec);

        private:
            x264_impl * impl_;
        };

        PPBOX_REGISTER_ENCODER(VideoSubType::AVC1, AvcEncoder);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_ENCODER_H_
