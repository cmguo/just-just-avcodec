// AvcByteStreamSplitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcByteStreamSplitter.h"
#include "ppbox/avcodec/avc/AvcEnum.h"

namespace ppbox
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
} // namespace ppbox
