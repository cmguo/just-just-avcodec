// AacRawAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacRawAssembler.h"
#include "ppbox/avcodec/aac/AacConfigHelper.h"

namespace ppbox
{
    namespace avcodec
    {

        AacRawAssembler::AacRawAssembler()
        {
        }

        AacRawAssembler::~AacRawAssembler()
        {
        }

        bool AacRawAssembler::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            AacConfigHelper const & config = *(AacConfigHelper const *)info.context;
            config.to_data(info.format_data);
            info.format_type = AacFormatType::raw;
            return true;
        }

        bool AacRawAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
