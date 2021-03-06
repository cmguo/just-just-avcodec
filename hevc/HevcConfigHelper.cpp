// HevcConfigHelper.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/hevc/HevcConfigHelper.h"
#include "just/avcodec/hevc/HevcConfig.h"
#include <just/avcodec/hevc/HevcNaluHeader.h>
#include <just/avcodec/hevc/HevcEnum.h>
#include "just/avcodec/nalu/NaluHelper.h"
#include <just/avcodec/nalu/NaluBuffer.h>
//#include <just/avcodec/hevc/HevcSpsPpsType.h>

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

        HevcConfigHelper::HevcConfigHelper()
            : data_(new HevcConfig)
        {
        }

        HevcConfigHelper::HevcConfigHelper(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
            : data_(new HevcConfig)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        HevcConfigHelper::HevcConfigHelper(
            std::vector<boost::uint8_t> const & vec)
            : data_(new HevcConfig)
        {
            from_data(vec);
        }

        HevcConfigHelper::~HevcConfigHelper()
        {
            delete data_;
        }

        void HevcConfigHelper::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            BitsIStream<boost::uint8_t> is(abuf);
            is >> *data_;
        }

        void HevcConfigHelper::to_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.resize(256);
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size());
            BitsOStream<boost::uint8_t> os(abuf);
            os << *data_;
            buf.resize(abuf.size());
        }

        void HevcConfigHelper::from_es_data(
            std::vector<boost::uint8_t> const & buf)
        {
            NaluHelper::buffers_t buffers;
            buffers.push_back(boost::asio::buffer(buf));
            NaluHelper helper(HevcNaluType::is_access_end);
            helper.from_stream(buf.size(), buffers);
            std::vector<NaluBuffer> const & nalus = helper.nalus();
            data_->arrays.clear();
            data_->arrays.resize(3);
            for (size_t i = 0; i < nalus.size(); ++i) {
                NaluBuffer const & nalu = nalus[i];
                HevcNaluHeader const nalu_header(nalu.begin.dereference_byte(), 0);
                std::vector<boost::uint8_t> nalu_bytes(nalu.bytes_begin(), nalu.bytes_end());
                if (nalu_header.nal_unit_type >= HevcNaluType::VPS_NUT 
                    && nalu_header.nal_unit_type <= HevcNaluType::PPS_NUT) {
                        HevcConfig::ArrayElem & array = data_->arrays[nalu_header.nal_unit_type - HevcNaluType::VPS_NUT];
                        array.nalUnitLength.push_back(nalu_bytes.size());
                        array.nalUnit.push_back(nalu_bytes);
                }
            }
            for (size_t i = data_->arrays.size() - 1; i != size_t(-1); --i) {
                if (data_->arrays[i].nalUnitLength.empty()) {
                    data_->arrays.erase(data_->arrays.begin() + i);
                } else {
                    data_->arrays[i].NAL_unit_type = HevcNaluType::VPS_NUT + i;
                }
            }
            if (!data_->arrays.empty()) {
                data_->configurationVersion = 1;
                    /*
                data_->HEVCProfileIndication = data_->sequenceParameterSetNALUnit[0][1];
                data_->profile_compatibility = 0;
                data_->HEVCLevelIndication = data_->sequenceParameterSetNALUnit[0][3];
                data_->reserved = 0x3f;
                data_->lengthSizeMinusOne = 3;
                data_->reserved2 = 0x7;
                data_->numOfSequenceParameterSets = data_->sequenceParameterSetNALUnit.size();
                data_->numOfPictureParameterSets = data_->pictureParameterSetNALUnit.size();
                */
            }
        }

        void HevcConfigHelper::to_es_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.clear();
            boost::uint8_t vec_0001[] = {0, 0, 0, 1};
            for (size_t i = 0; i < data_->arrays.size(); ++i) {
                HevcConfig::ArrayElem const & array = data_->arrays[i];
                for (boost::uint32_t j = 0; j < array.nalUnitLength.size(); j++) {
                    buf.insert(buf.end(), vec_0001, vec_0001 + 4);
                    buf.insert(buf.end(), array.nalUnit[j].begin(), array.nalUnit[j].end());
                }
            }
        }

        HevcConfigHelper::param_set_t const & HevcConfigHelper::param_set(
            boost::uint8_t type) const
        {
            for (size_t i = 0; i < data_->arrays.size(); ++i) {
                HevcConfig::ArrayElem const & array = data_->arrays[i];
                if (array.NAL_unit_type == type)
                    return array.nalUnit;
            }
            static param_set_t empty;
            return empty;
        }

        HevcConfigHelper::param_set_t const & HevcConfigHelper::vps() const
        {
            return param_set(HevcNaluType::VPS_NUT);
        }

        HevcConfigHelper::param_set_t const & HevcConfigHelper::sps() const
        {
            return param_set(HevcNaluType::SPS_NUT);
        }

        HevcConfigHelper::param_set_t const & HevcConfigHelper::pps() const
        {
            return param_set(HevcNaluType::PPS_NUT);
        }

        bool HevcConfigHelper::ready() const
        {
            return !data_->arrays.empty();
        }

        void HevcConfigHelper::get_format(
            VideoInfo & info) const
        {
            if (!ready()) {
                return;
            }
            /*
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
                info.frame_rate_num = sps.vui_parameters.time_scale;
                info.frame_rate_den = sps.vui_parameters.num_units_in_tick;
            } else {
                info.frame_rate_num = 0;
                info.frame_rate_den = 0;
            }
            */
        }

    } // namespace avcodec
} // namespace just
