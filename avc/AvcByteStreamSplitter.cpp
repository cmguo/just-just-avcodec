// AvcByteStreamSplitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcByteStreamSplitter.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcByteStreamSplitter::AvcByteStreamSplitter()
        {
        }

        AvcByteStreamSplitter::~AvcByteStreamSplitter()
        {
        }

        bool AvcByteStreamSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            info.context = &config_;
            return true;
        }

        bool AvcByteStreamSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            AvcConfigHelper const & config = *(AvcConfigHelper const *)sample.stream_info->context;
            sample.context = &helper_;
            return helper_.from_stream(sample.size, sample.data);
        }

    } // namespace avcodec
} // namespace ppbox
