// AvcByteStreamSplitter.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/avc/AvcByteStreamSplitter.h"
#include "just/avcodec/avc/AvcEnum.h"

namespace just
{
    namespace avcodec
    {

        AvcByteStreamSplitter::AvcByteStreamSplitter()
            : helper_(AvcNaluType::is_access_end)
        {
        }

        AvcByteStreamSplitter::~AvcByteStreamSplitter()
        {
        }

        bool AvcByteStreamSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_es_data(info.format_data);
            info.context = &config_;
            return true;
        }

        bool AvcByteStreamSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            sample.context = &helper_;
            return helper_.from_stream(sample.size, sample.data);
        }

    } // namespace avcodec
} // namespace just
