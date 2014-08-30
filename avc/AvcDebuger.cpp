// AvcDebuger.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcDebuger.h"
#include "ppbox/avcodec/avc/AvcConfig.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"
#include "ppbox/avcodec/avc/AvcType.h"
#include "ppbox/avcodec/avc/AvcEnum.h"
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

        std::map<boost::uint32_t, SeqParameterSetRbsp> spss;
        std::map<boost::uint32_t, PicParameterSetRbsp> ppss;

        bool AvcDebuger::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            AvcConfig const & config = 
                ((AvcConfigHelper const *)info.context)->data();
            std::cout << "[Avc Config] profile: " << config.AVCProfileIndication
                << " level: " << config.AVCLevelIndication << std::endl;
            for (boost::uint32_t i = 0; i < config.sequenceParameterSetNALUnit.size(); i++) {
                std::cout << "  [SPS] size: " << config.sequenceParameterSetNALUnit[i].size() << std::endl;
                std::vector<boost::uint8_t> sps_vec = config.sequenceParameterSetNALUnit[i];
                FormatBuffer buf((boost::uint8_t *)&sps_vec[0], sps_vec.size(), sps_vec.size());
                BitsBuffer<boost::uint8_t> bits_buf(buf);
                BitsIStream<boost::uint8_t> bits_reader(bits_buf);
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
            }
            // pps
            for (boost::uint32_t i = 0; i < config.pictureParameterSetNALUnit.size(); i++) {
                std::cout << "  [PPS] size: " << config.pictureParameterSetNALUnit[i].size() << std::endl;
                std::vector<boost::uint8_t> const & pps_vec = config.pictureParameterSetNALUnit[i];
                FormatBuffer buf((boost::uint8_t *)&pps_vec[0], pps_vec.size(), pps_vec.size());
                BitsBuffer<boost::uint8_t> bits_buf(buf);
                BitsIStream<boost::uint8_t> bits_reader(bits_buf);
                PicParameterSetRbsp pps(spss);
                bits_reader >> pps;
                ppss.insert(std::make_pair(pps.pps_pic_parameter_set_id, pps));
            }

            return true;
        }

        bool AvcDebuger::debug(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            NaluHelper & helper = *(NaluHelper *)sample.context;
            std::vector<ppbox::avcodec::NaluBuffer> const & nalus = helper.nalus();
            for (boost::uint32_t i = 0; i < nalus.size(); ++i) {
                NaluBuffer const & nalu = nalus[i];
                AvcNaluHeader nalu_header(nalu.begin.dereference_byte());
                std::cout << "  [Nalu] type: " << AvcNaluType::type_str[nalu_header.nal_unit_type] << std::endl;

                if (nalu_header.nal_unit_type == AvcNaluType::SEI) {
                    std::cout << "    [Sei]: size = " << nalus[i].size << std::endl;
                    util::buffers::CycleBuffers<NaluBuffer::RangeBuffers, boost::uint8_t> buf(nalu.buffers());
                    buf.commit(nalus[i].size);
                    BitsBuffer<boost::uint8_t> bits_buf(buf);
                    BitsIStream<boost::uint8_t> bits_reader(bits_buf);
                    SeiRbsp sei(spss, ppss);
                    bits_reader >> sei;
                }
                if (nalu_header.nal_unit_type == AvcNaluType::UNIDR
                    || nalu_header.nal_unit_type == AvcNaluType::IDR) {
                        util::buffers::CycleBuffers<NaluBuffer::RangeBuffers, boost::uint8_t> buf(nalu.buffers());
                        buf.commit(nalus[i].size);
                        BitsBuffer<boost::uint8_t> bits_buf(buf);
                        BitsIStream<boost::uint8_t> bits_reader(bits_buf);
                        SliceLayerWithoutPartitioningRbsp slice(ppss);
                        bits_reader >> slice;
                        std::cout << "    [Slice] type: " 
                            << AvcSliceType::type_str[slice.slice_header.slice_type] 
                        << std::endl;
                }
            }

            return true;
        }

    } // namespace avcodec
} // namespace ppbox
