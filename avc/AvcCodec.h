// AvcCodec.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_CODEC_H_
#define _PPBOX_AVCODEC_AVC_AVC_CODEC_H_

#include "ppbox/avcodec/Codec.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"
#include "ppbox/avcodec/Format.h"

namespace ppbox
{
    namespace avcodec
    {

        class AvcCodec
            : public Codec
        {
        public:
            AvcCodec();

            AvcCodec(
                boost::uint32_t format, 
                std::vector<boost::uint8_t> const & config);

        public:
            AvcConfigHelper const & config_helper() const
            {
                return config_helper_;
            }

            AvcConfig const & config() const
            {
                return config_helper_.data();
            }

        private:
            AvcConfigHelper config_helper_;
        };

        PPBOX_REGISTER_CODEC(VideoSubType::AVC1, AvcCodec);

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_CODEC_H_
