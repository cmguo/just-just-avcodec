// AacRawSplitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/aac/AacRawSplitter.h"

namespace ppbox
{
    namespace avcodec
    {

        AacRawSplitter::AacRawSplitter()
        {
        }

        AacRawSplitter::~AacRawSplitter()
        {
        }

        bool AacRawSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_adts_data(info.format_data);
            info.context = &config_;
            return true;
        }

        bool AacRawSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
