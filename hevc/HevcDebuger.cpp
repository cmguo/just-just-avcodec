// HevcDebuger.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/hevc/HevcDebuger.h"
#include "ppbox/avcodec/hevc/HevcConfig.h"
#include "ppbox/avcodec/hevc/HevcConfigHelper.h"
#include "ppbox/avcodec/hevc/HevcType.h"
#include "ppbox/avcodec/hevc/HevcEnum.h"
#include "ppbox/avcodec/hevc/HevcNaluHeader.h"
#include "ppbox/avcodec/nalu/NaluHelper.h"
#include "ppbox/avcodec/nalu/NaluBuffer.h"

#include <ppbox/avbase/stream/BitsOStream.h>
#include <ppbox/avbase/stream/BitsIStream.h>
#include <ppbox/avbase/stream/BitsBuffer.h>
#include <ppbox/avbase/stream/FormatBuffer.h>
using namespace ppbox::avbase;

#include <util/archive/ArchiveBuffer.h>
#include <util/buffers/CycleBuffers.h>

namespace ppbox
{
    namespace avcodec
    {

        //std::map<boost::uint32_t, SeqParameterSetRbsp> spss;
        //std::map<boost::uint32_t, PicParameterSetRbsp> ppss;

        bool HevcDebuger::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            HevcConfig const & config = 
                ((HevcConfigHelper const *)info.context)->data();
            std::cout << "[Hevc Config] profile: " << config.general_profile_idc
                << " level: " << config.general_level_idc << std::endl;
            for (boost::uint32_t i = 0; i < config.arrays.size(); i++) {
                HevcConfig::ArrayElem const & array = config.arrays[i];
                std::cout << "  [Array] type: " << HevcNaluType::type_str[array.NAL_unit_type]
                    << " size: " << array.nalUnitLength.size() << std::endl;
                for (boost::uint32_t j = 0; j < array.nalUnitLength.size(); j++) {
                    std::cout << "    [Nalu] size: " << array.nalUnit[j].size() << std::endl;
                    std::vector<boost::uint8_t> sps_vec = array.nalUnit[j];
                    FormatBuffer buf((boost::uint8_t *)&sps_vec[0], sps_vec.size(), sps_vec.size());
                    BitsBuffer<boost::uint8_t> bits_buf(buf);
                    BitsIStream<boost::uint8_t> bits_reader(bits_buf);
                    /*
                    SeqParameterSetRbsp sps;
                    bits_reader >> sps;
                    //if (!sps.vui_parameters.fixed_frame_rate_flag) {
                    //    sps.vui_parameters.num_units_in_tick = 1;
                    //    sps.vui_parameters.time_scale = 50;
                    //    sps.vui_parameters.fixed_frame_rate_flag = 1;
                    //    sps_vec.resize(sps_vec.size() + 4);
                    //    FormatBuffer buf((boost::uint8_t *)&sps_vec[0], sps_vec.size());
                    //    BitsBuffer<boost::uint8_t> bits_buf(buf);
                    //    BitsOStream<boost::uint8_t> bits_writer(bits_buf);
                    //    bits_writer << sps;
                    //    sps_vec.resize(buf.size());
                    //    
                    //}
                    spss.insert(std::make_pair(sps.sps_seq_parameter_set_id, sps));
                    */
                }
            }

            return true;
        }

        bool HevcDebuger::debug(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            NaluHelper & helper = *(NaluHelper *)sample.context;
            std::vector<ppbox::avcodec::NaluBuffer> const & nalus = helper.nalus();
            for (boost::uint32_t i = 0; i < nalus.size(); ++i) {
                NaluBuffer const & nalu = nalus[i];
                HevcNaluHeader nalu_header(nalu.begin.dereference_byte(), 0);
                std::cout << "  [Nalu] type: " << HevcNaluType::type_str[nalu_header.nal_unit_type] 
                    << " size: " << nalu.size << std::endl;
                /*
                if (nalu_header.nal_unit_type == HevcNaluType::SEI) {
                    std::cout << "    [Sei]: size = " << nalus[i].size << std::endl;
                    util::buffers::CycleBuffers<NaluBuffer::RangeBuffers, boost::uint8_t> buf(nalu.buffers());
                    buf.commit(nalus[i].size);
                    BitsBuffer<boost::uint8_t> bits_buf(buf);
                    BitsIStream<boost::uint8_t> bits_reader(bits_buf);
                    SeiRbsp sei(spss, ppss);
                    bits_reader >> sei;
                }
                if (nalu_header.nal_unit_type == HevcNaluType::UNIDR
                    || nalu_header.nal_unit_type == HevcNaluType::IDR) {
                        util::buffers::CycleBuffers<NaluBuffer::RangeBuffers, boost::uint8_t> buf(nalu.buffers());
                        buf.commit(nalus[i].size);
                        BitsBuffer<boost::uint8_t> bits_buf(buf);
                        BitsIStream<boost::uint8_t> bits_reader(bits_buf);
                        SliceLayerWithoutPartitioningRbsp slice(ppss);
                        bits_reader >> slice;
                        std::cout << "    [Slice] type: " 
                            << SliceHeader::slice_type_str[slice.slice_header.slice_type] 
                        << std::endl;
                }
                */
            }

            return true;
        }

    } // namespace avcodec
} // namespace ppbox
