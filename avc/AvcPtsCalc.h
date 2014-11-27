// AvcPtsCalc.h

#ifndef _JUST_AVCODEC_AVC_AVC_PTS_CALC_H_
#define _JUST_AVCODEC_AVC_AVC_PTS_CALC_H_

#include "just/avcodec/avc/AvcSpsPpsType.h"
#include "just/avcodec/Base.h"

namespace just
{
    namespace avcodec
    {

        class AvcPtsCalc
        {
        public:
            AvcPtsCalc();

        public:
            virtual void calc(
                Sample & sample);

        private:
            std::map<boost::uint32_t, SeqParameterSetRbsp> spss_;
            std::map<boost::uint32_t, PicParameterSetRbsp> ppss_;
            boost::uint64_t idr_dts_;
            boost::uint32_t frame_scale_;
            bool is_last_a_idr_;
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AVC_AVC_PTS_CALC_H_
