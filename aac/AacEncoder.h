// AacEncoder.h

#ifndef _PPBOX_AACODEC_AAC_AAC_ENCODER_H_
#define _PPBOX_AACODEC_AAC_AAC_ENCODER_H_

#include "ppbox/avcodec/Encoder.h"
#include "ppbox/avcodec/CodecType.h"

namespace ppbox
{
    namespace avcodec
    {

        struct fdk_aac_impl;

        class AacEncoder
            : public Encoder
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

        PPBOX_REGISTER_ENCODER(AudioSubType::MP4A, AacEncoder);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AACODEC_AAC_AAC_ENCODER_H_
