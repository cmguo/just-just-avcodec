// AvcConfigHelper.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/avc/AvcConfigHelper.h"
#include "just/avcodec/avc/AvcConfig.h"
#include "just/avcodec/avc/AvcSpsPpsType.h"
#include "just/avcodec/avc/AvcEnum.h"
#include "just/avcodec/avc/AvcNaluHeader.h"
#include "just/avcodec/nalu/NaluHelper.h"
#include "just/avcodec/nalu/NaluBuffer.h"

#include <just/avbase/stream/BitsIStream.h>
#include <just/avbase/stream/BitsOStream.h>
#include <just/avbase/stream/BitsBuffer.h>
#include <just/avbase/stream/FormatBuffer.h>
#include <just/avbase/StreamInfo.h>
using namespace just::avbase;

namespace just
{
    namespace avcodec
    {

        AvcConfigHelper::AvcConfigHelper()
            : data_(new AvcConfig)
        {
        }

        AvcConfigHelper::AvcConfigHelper(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
            : data_(new AvcConfig)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        AvcConfigHelper::AvcConfigHelper(
            std::vector<boost::uint8_t> const & vec)
            : data_(new AvcConfig)
        {
            from_data(vec);
        }

        AvcConfigHelper::~AvcConfigHelper()
        {
            delete data_;
        }

        void AvcConfigHelper::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            BitsIStream<boost::uint8_t> is(abuf);
            is >> *data_;
        }

        void AvcConfigHelper::to_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.resize(256);
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size());
            BitsOStream<boost::uint8_t> os(abuf);
            os << *data_;
            buf.resize(abuf.size());
        }

        void AvcConfigHelper::from_es_data(
            std::vector<boost::uint8_t> const & buf)
        {
            NaluHelper::buffers_t buffers;
            buffers.push_back(boost::asio::buffer(buf));
            NaluHelper helper(AvcNaluType::is_access_end);
            helper.from_stream(buf.size(), buffers);
            std::vector<NaluBuffer> const & nalus = helper.nalus();
            data_->sequenceParameterSetLength.clear();
            data_->sequenceParameterSetNALUnit.clear();
            data_->pictureParameterSetLength.clear();
            data_->pictureParameterSetNALUnit.clear();
            for (size_t i = 0; i < nalus.size(); ++i) {
                NaluBuffer const & nalu = nalus[i];
                AvcNaluHeader const nalu_header(nalu.begin.dereference_byte());
                std::vector<boost::uint8_t> nalu_bytes(nalu.bytes_begin(), nalu.bytes_end());
                if (nalu_header.nal_unit_type == AvcNaluType::SPS) {
                    data_->sequenceParameterSetLength.push_back(nalu_bytes.size());
                    data_->sequenceParameterSetNALUnit.push_back(nalu_bytes);
                } else if (nalu_header.nal_unit_type == AvcNaluType::PPS) {
                    data_->pictureParameterSetLength.push_back(nalu_bytes.size());
                    data_->pictureParameterSetNALUnit.push_back(nalu_bytes);
                }
            }
            if (!data_->sequenceParameterSetNALUnit.empty()) {
                data_->configurationVersion = 1;
                data_->AVCProfileIndication = data_->sequenceParameterSetNALUnit[0][1];
                data_->profile_compatibility = 0;
                data_->AVCLevelIndication = data_->sequenceParameterSetNALUnit[0][3];
                data_->reserved = 0x3f;
                data_->lengthSizeMinusOne = 3;
                data_->reserved2 = 0x7;
                data_->numOfSequenceParameterSets = data_->sequenceParameterSetNALUnit.size();
                data_->numOfPictureParameterSets = data_->pictureParameterSetNALUnit.size();
            }
        }

        void AvcConfigHelper::to_es_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.clear();
            boost::uint8_t vec_0001[] = {0, 0, 0, 1};
            for (size_t i = 0; i < data_->sequenceParameterSetNALUnit.size(); ++i) {
                buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                buf.insert(buf.end(), data_->sequenceParameterSetNALUnit[i].begin(), data_->sequenceParameterSetNALUnit[i].end());
            }
            for (size_t i = 0; i < data_->pictureParameterSetNALUnit.size(); ++i) {
                buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                buf.insert(buf.end(), data_->pictureParameterSetNALUnit[i].begin(), data_->pictureParameterSetNALUnit[i].end());
            }
        }

        bool AvcConfigHelper::ready() const
        {
            return !data_->sequenceParameterSetNALUnit.empty();
        }

        void AvcConfigHelper::get_format(
            VideoInfo & info) const
        {
            if (!ready()) {
                return;
            }
            SeqParameterSetRbsp sps;
            util::archive::ArchiveBuffer<boost::uint8_t> abuf(
                &data_->sequenceParameterSetNALUnit[0].front(), 
                data_->sequenceParameterSetLength[0], 
                data_->sequenceParameterSetLength[0]);
            BitsBuffer<boost::uint8_t> buf(abuf);
            BitsIStream<boost::uint8_t> is(buf);
            is >> sps;

            info.width = (sps.pic_width_in_mbs_minus1 + 1) * 16 
                - (sps.frame_crop_left_offset + sps.frame_crop_right_offset) * 2;
            info.height = (2 - sps.frame_mbs_only_flag) * (sps.pic_height_in_map_units_minus1 + 1) * 16
                - (sps.frame_crop_top_offset + sps.frame_crop_bottom_offset) * 2;
            if (sps.vui_parameters.timing_info_present_flag) {
                if ((boost::uint64_t)info.frame_rate_num * sps.vui_parameters.num_units_in_tick * 2 
                    != (boost::uint64_t)sps.vui_parameters.time_scale * info.frame_rate_den) {
                        info.frame_rate_num = sps.vui_parameters.time_scale;
                        info.frame_rate_den = sps.vui_parameters.num_units_in_tick;
                }
            }
        }

    } // namespace avcodec
} // namespace just
