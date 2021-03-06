// AacAdtsSplitter.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/aac/AacAdtsSplitter.h"
#include "just/avcodec/aac/AacAdts.h"

#include <just/avbase/stream/BitsIStream.h>
#include <just/avbase/stream/SampleBuffers.h>
using namespace just::avbase;

#include <util/buffers/CycleBuffers.h>

namespace just
{
    namespace avcodec
    {

        AacAdtsSplitter::AacAdtsSplitter()
        {
        }

        AacAdtsSplitter::~AacAdtsSplitter()
        {
        }

        bool AacAdtsSplitter::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            config_.from_adts_data(info.format_data);
            info.context = &config_;
            return true;
        }

        bool AacAdtsSplitter::split(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            util::buffers::CycleBuffers<std::deque<boost::asio::const_buffer>, boost::uint8_t> buf(sample.data);
            buf.commit(sample.size);
            BitsIStream<boost::uint8_t> bits_is(buf);

            SampleBuffers::BuffersPosition position(sample.data.begin(), sample.data.end());
            SampleBuffers::BuffersPosition end(sample.data.end());

            std::deque<boost::asio::const_buffer> data;

            while (!position.at_end()) {
                AacAdts adts;
                bits_is >> adts;
                size_t adts_size = buf.in_position() - position.skipped_bytes();
                sample.size -= adts_size;
                SampleBuffers::BuffersPosition position2(position);
                position2.increment_bytes(end, adts_size);;
                position.increment_bytes(end, adts.frame_length);
                bits_is.seekg((size_t)adts.frame_length - adts_size, std::ios::cur);
                data.insert(data.end(), SampleBuffers::range_buffers_begin(position2, position), SampleBuffers::range_buffers_end());
            }

            sample.data.swap(data);

            return true;
        }

    } // namespace avcodec
} // namespace just
