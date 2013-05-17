// AvcPacketAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcPacketAssembler.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"
#include "ppbox/avcodec/avc/AvcNaluHelper.h"

namespace ppbox
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
            return true;
        }

        bool AvcPacketAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            AvcNaluHelper & helper = *(AvcNaluHelper *)sample.context;
            sample.size = 0;
            NaluBuffer::ConstBuffers data;
            bool b = helper.to_packet(sample.size, data);
            sample.data.swap(data);
            sample.context = NULL;
            return b;
        }

    } // namespace avcodec
} // namespace ppbox
