// HevcByteStreamSplitter.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/hevc/HevcByteStreamSplitter.h"
#include "just/avcodec/hevc/HevcEnum.h"

namespace just
{
    namespace avcodec
    {

        HevcByteStreamSplitter::HevcByteStreamSplitter()
            : helper_(HevcNaluType::is_access_end)
        {
        }

        HevcByteStreamSplitter::~HevcByteStreamSplitter()
        {
        }

        bool HevcByteStreamSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_es_data(info.format_data);
            info.context = &config_;
            return true;
        }

        bool HevcByteStreamSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            sample.context = &helper_;
            return helper_.from_stream(sample.size, sample.data);
        }

    } // namespace avcodec
} // namespace just
