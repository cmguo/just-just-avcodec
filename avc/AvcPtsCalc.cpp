// AvcPtsCalc.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/avc/AvcPtsCalc.h"
#include "just/avcodec/avc/AvcSliceType.h"
#include "just/avcodec/avc/AvcEnum.h"
#include "just/avcodec/nalu/NaluBuffer.h"

#include <just/avbase/stream/BitsIStream.h>
#include <just/avbase/stream/BitsOStream.h>
#include <just/avbase/stream/FormatBuffer.h>
#include <just/avbase/stream/BitsBuffer.h>
using namespace just::avbase;

#include <util/buffers/CycleBuffers.h>

namespace just
{
    namespace avcodec
    {

        template <typename T>
        static bool parse(
            T & t, 
            Sample & s, 
            NaluBuffer const & n)
        {
            util::buffers::CycleBuffers<SampleBuffers::RangeBuffers, boost::uint8_t> buf(n.buffers());
            buf.commit(n.size);
            BitsBuffer<boost::uint8_t> bits_buf(buf);
            BitsIStream<boost::uint8_t> bits_reader(bits_buf);
            bits_reader >> t;
            return !!bits_reader;
        }

        AvcPtsCalc::AvcPtsCalc()
            : idr_dts_(0)
            , frame_scale_(0)
            , is_last_a_idr_(false)
        {
        }

        void AvcPtsCalc::calc(
            Sample & sample)
        {
            if (sample.cts_delta != boost::uint32_t(-1)) {
                return;
            }

            std::vector<NaluBuffer> & nalus = 
                *(std::vector<NaluBuffer> *)sample.context;

            if (sample.flags & Sample::f_sync) {
                idr_dts_ = sample.dts;
                is_last_a_idr_ = true;
            } else if (is_last_a_idr_) {
                frame_scale_ = boost::uint32_t(sample.dts - idr_dts_);
                is_last_a_idr_ = false;
            }
            for (size_t i = 0; i < nalus.size(); ++i) {
                NaluBuffer const & nalu = nalus[i];
                AvcNaluHeader nalu_header(nalu.begin.dereference_byte());
                if (AvcNaluType::SPS == nalu_header.nal_unit_type) {
                    SeqParameterSetRbsp sps;
                    parse(sps, sample, nalu);
                    spss_.insert(std::make_pair(sps.sps_seq_parameter_set_id, sps));
                } else if (AvcNaluType::PPS == nalu_header.nal_unit_type) {
                    PicParameterSetRbsp pps(spss_);
                    parse(pps, sample, nalu);
                    ppss_.insert(std::make_pair(pps.pps_pic_parameter_set_id, pps));
                } else if (AvcNaluType::UNIDR == nalu_header.nal_unit_type
                    || AvcNaluType::IDR == nalu_header.nal_unit_type) {
                    SliceLayerWithoutPartitioningRbsp slice(ppss_);
                    parse(slice, sample, nalu);
                    sample.cts_delta = (boost::uint32_t)(idr_dts_ + frame_scale_ * slice.slice_header.pic_order_cnt_lsb / 2 - sample.dts);
                    // iphone¼��ʹ��
                    if (slice.slice_header.slice_type % 5 == 2) {
                        sample.flags |= Sample::f_sync;
                    }
                } else {
                    // skip
                }
            } // End for
        }

    } // namespace avcodec
} // namespace just
