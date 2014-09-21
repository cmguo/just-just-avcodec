// AvcPtsCalc.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_PTS_CALC_H_
#define _PPBOX_AVCODEC_AVC_AVC_PTS_CALC_H_

#include "ppbox/avcodec/avc/AvcSpsPpsType.h"
#include "ppbox/avcodec/Base.h"

namespace ppbox
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
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_PTS_CALC_H_
