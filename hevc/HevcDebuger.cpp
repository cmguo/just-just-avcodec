// HevcDebuger.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/hevc/HevcDebuger.h"
#include "just/avcodec/hevc/HevcConfig.h"
#include "just/avcodec/hevc/HevcConfigHelper.h"
#include "just/avcodec/hevc/HevcType.h"
#include "just/avcodec/hevc/HevcEnum.h"
#include "just/avcodec/hevc/HevcNaluHeader.h"
#include "just/avcodec/nalu/NaluHelper.h"
#include "just/avcodec/nalu/NaluBuffer.h"

#include <just/avbase/stream/BitsOStream.h>
#include <just/avbase/stream/BitsIStream.h>
#include <just/avbase/stream/BitsBuffer.h>
#include <just/avbase/stream/FormatBuffer.h>
using namespace just::avbase;

#include <util/archive/ArchiveBuffer.h>
#include <util/buffers/CycleBuffers.h>

namespace just
{
    namespace avcodec
    {

        //std::map<boost::uint32_t, SeqParameterSetRbsp> spss;
        //std::map<boost::uint32_t, PicParameterSetRbsp> ppss;

        bool HevcDebuger::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            Debuger::reset(info, ec);

            HevcConfig const & config = 
                ((HevcConfigHelper const *)info.context)->data();
            std::cout << "[Hevc Config] profile: " << config.general_profile_idc
                << " level: " << config.general_level_idc << std::endl;
            char start_code[] = {0, 0, 0, 1};
            for (boost::uint32_t i = 0; i < config.arrays.size(); i++) {
                HevcConfig::ArrayElem const & array = config.arrays[i];
                std::cout << "  [Array] type: " << HevcNaluType::type_str[array.NAL_unit_type]
                    << " size: " << array.nalUnitLength.size() << std::endl;
                for (boost::uint32_t j = 0; j < array.nalUnitLength.size(); j++) {
                    std::cout << "    [Nalu] size: " << array.nalUnit[j].size() << std::endl;
                    std::vector<boost::uint8_t> const & ps = array.nalUnit[j];

                    dump(boost::asio::buffer(start_code, 4));
                    dump(boost::asio::buffer(ps));
                }
            }

            return true;
        }

        bool HevcDebuger::debug(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            NaluHelper & helper = *(NaluHelper *)sample.context;
            std::vector<just::avcodec::NaluBuffer> const & nalus = helper.nalus();
            char start_code[] = {0, 0, 0, 1};
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

                if (i == 0)
                    dump(boost::asio::buffer(start_code, 4));
                else
                    dump(boost::asio::buffer(start_code + 1, 3));
                dump(nalu.buffers());
            }

            return true;
        }

    } // namespace avcodec
} // namespace just
