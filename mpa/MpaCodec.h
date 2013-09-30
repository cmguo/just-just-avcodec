// MpaCodec.h

#ifndef _PPBOX_AVCODEC_MPA_MPA_CODEC_H_
#define _PPBOX_AVCODEC_MPA_MPA_CODEC_H_

#include "ppbox/avcodec/Codec.h"
#include "ppbox/avcodec/mpa/MpaConfigHelper.h"

namespace ppbox
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

        PPBOX_REGISTER_CODEC(AudioSubType::MP1A, MpaCodec);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_MPA_MPA_CODEC_H_
