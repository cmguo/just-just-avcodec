// AacCodec.h

#ifndef _JUST_AVCODEC_AAC_AAC_CODEC_H_
#define _JUST_AVCODEC_AAC_AAC_CODEC_H_

#include "just/avcodec/Codec.h"
#include "just/avcodec/AudioType.h"
#include "just/avcodec/aac/AacConfigHelper.h"

namespace just
{
    namespace avcodec
    {

        class AacCodec
            : public Codec
        {
        public:
            AacCodec();

            AacCodec(
                boost::uint32_t format, 
                std::vector<boost::uint8_t> const & config);

        public:
            virtual bool finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec);

        public:
            AacConfigHelper const & config_helper() const
            {
                return config_helper_;
            }

            AacConfig const & config() const
            {
                return config_helper_.data();
            }

        private:
            AacConfigHelper config_helper_;
        };

        JUST_REGISTER_CODEC(AudioType::AAC,  AacCodec);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_CODEC_H_
