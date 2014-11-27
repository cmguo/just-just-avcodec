// AacRawSplitter.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/aac/AacRawSplitter.h"

namespace just
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
            config_.from_data(info.format_data);
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
} // namespace just
