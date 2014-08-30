// HevcByteStreamSplitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/hevc/HevcByteStreamSplitter.h"
#include "ppbox/avcodec/hevc/HevcEnum.h"

namespace ppbox
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
} // namespace ppbox
