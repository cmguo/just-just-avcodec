// AvcPacketAssembler.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/avc/AvcPacketAssembler.h"
#include "just/avcodec/avc/AvcConfigHelper.h"
#include "just/avcodec/avc/AvcEnum.h"
#include "just/avcodec/nalu/NaluHelper.h"

namespace just
{
    namespace avcodec
    {

        AvcPacketAssembler::AvcPacketAssembler()
        {
        }

        AvcPacketAssembler::~AvcPacketAssembler()
        {
        }

        bool AvcPacketAssembler::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            AvcConfigHelper const & config = *(AvcConfigHelper const *)info.context;
            config.to_data(info.format_data);
            info.format_type = AvcFormatType::packet;
            return true;
        }

        static bool nalu_is_seq_aud(
            NaluBuffer const & nalu)
        {
            return AvcNaluType::is_seq_aud(nalu.begin.dereference_byte());
        }

        bool AvcPacketAssembler::assemble(
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
} // namespace just
