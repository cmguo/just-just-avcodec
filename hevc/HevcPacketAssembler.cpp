// HevcPacketAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/hevc/HevcPacketAssembler.h"
#include "ppbox/avcodec/hevc/HevcConfigHelper.h"
#include "ppbox/avcodec/hevc/HevcEnum.h"
#include "ppbox/avcodec/hevc/HevcNaluHeader.h"
#include "ppbox/avcodec/nalu/NaluHelper.h"

namespace ppbox
{
    namespace avcodec
    {

        HevcPacketAssembler::HevcPacketAssembler()
        {
        }

        HevcPacketAssembler::~HevcPacketAssembler()
        {
        }

        bool HevcPacketAssembler::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            HevcConfigHelper const & config = *(HevcConfigHelper const *)info.context;
            config.to_data(info.format_data);
            info.format_type = HevcFormatType::packet;
            return true;
        }

        static bool nalu_is_seq_aud(
            NaluBuffer const & nalu)
        {
            return HevcNaluType::is_seq_aud(nalu.begin.dereference_byte());
        }

        bool HevcPacketAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            NaluHelper & helper = *(NaluHelper *)sample.context;
            std::vector<NaluBuffer> & nalus = helper.nalus();
            nalus.erase(std::remove_if(nalus.begin(), nalus.end(), nalu_is_seq_aud), nalus.end());
            sample.size = 0;
            NaluBuffer::ConstBuffers data;
            bool b = helper.to_packet(sample.size, data);
            sample.data.swap(data);
            sample.context = NULL;
            return b;
        }

    } // namespace avcodec
} // namespace ppbox
