// AacCodec.h

#ifndef _PPBOX_AVCODEC_AAC_AAC_CODEC_H_
#define _PPBOX_AVCODEC_AAC_AAC_CODEC_H_

#include "ppbox/avcodec/Codec.h"
#include "ppbox/avcodec/aac/AacConfigHelper.h"
#include "ppbox/avcodec/Format.h"

namespace ppbox
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

        PPBOX_REGISTER_CODEC(AudioSubType::MP4A, AacCodec);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AAC_AAC_CODEC_H_
