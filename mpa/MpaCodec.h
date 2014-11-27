// MpaCodec.h

#ifndef _JUST_AVCODEC_MPA_MPA_CODEC_H_
#define _JUST_AVCODEC_MPA_MPA_CODEC_H_

#include "just/avcodec/Codec.h"
#include "just/avcodec/AudioType.h"
#include "just/avcodec/mpa/MpaConfigHelper.h"

namespace just
{
    namespace avcodec
    {

        class MpaCodec
            : public Codec
        {
        public:
            MpaCodec();

            MpaCodec(
                boost::uint32_t format, 
                std::vector<boost::uint8_t> const & config);

        public:
            virtual bool finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec);

        public:
            MpaConfigHelper const & config_helper() const
            {
                return config_helper_;
            }

            MpaConfig const & config() const
            {
                return config_helper_.data();
            }

        private:
            MpaConfigHelper config_helper_;
        };

        JUST_REGISTER_CODEC(AudioType::MP1A, MpaCodec);
        JUST_REGISTER_CODEC(AudioType::MP2A, MpaCodec);
        JUST_REGISTER_CODEC(AudioType::MP2, MpaCodec);
        JUST_REGISTER_CODEC(AudioType::MP3, MpaCodec);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_MPA_MPA_CODEC_H_
