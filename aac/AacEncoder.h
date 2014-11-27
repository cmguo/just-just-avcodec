// AacEncoder.h

#ifndef _JUST_AACODEC_AAC_AAC_ENCODER_H_
#define _JUST_AACODEC_AAC_AAC_ENCODER_H_

#include "just/avcodec/Transcoder2.h"

namespace just
{
    namespace avcodec
    {

        struct fdk_aac_impl;

        class AacEncoder
            : public Transcoder2
        {
        public:
            AacEncoder();

            virtual ~AacEncoder();

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
            fdk_aac_impl * impl_;
        };

        JUST_REGISTER_TRANSCODER("aac-enc", 10, AacEncoder);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AACODEC_AAC_AAC_ENCODER_H_
