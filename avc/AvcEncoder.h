// AvcEncoder.h

#ifndef _JUST_AVCODEC_AVC_AVC_ENCODER_H_
#define _JUST_AVCODEC_AVC_AVC_ENCODER_H_

#include "just/avcodec/Transcoder2.h"

namespace just
{
    namespace avcodec
    {

        struct x264_impl;

        class AvcEncoder
            : public Transcoder2
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

        JUST_REGISTER_TRANSCODER("avc-enc", 10, AvcEncoder);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_ENCODER_H_
