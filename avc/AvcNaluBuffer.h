// AvcNaluBuffer.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_NALU_BUFFER_H_
#define _PPBOX_AVCODEC_AVC_AVC_NALU_BUFFER_H_

#include <ppbox/avbase/stream/SampleBuffers.h>

namespace ppbox
{
    namespace avcodec
    {

        struct NaluBuffer
            : ppbox::avbase::SampleBuffers
        {
            NaluBuffer(
                boost::uint32_t s, 
                BuffersPosition const & b,
                BuffersPosition const & e)
                : size(s)
                , begin(b)
                , end(e)
            {
            }

            NaluBuffer(
                BuffersPosition const & b,
                BuffersPosition const & e)
                : size(e.skipped_bytes() - b.skipped_bytes())
                , begin(b)
                , end(e)
            {
            }

            BufferIterator buffers_begin() const
            {
                return BufferIterator(begin, end);
            }

            BufferIterator buffers_end() const
            {
                return BufferIterator();
            }

            ByteIterator bytes_begin() const
            {
                return ByteIterator(begin, end);
            }

            ByteIterator bytes_end() const
            {
                return ByteIterator();
            }

            RangeBuffers buffers() const
            {
                return SampleBuffers::range_buffers(begin, end);
            }

            RangeBytes bytes() const
            {
                return SampleBuffers::range_bytes(begin, end);
            }

            boost::uint32_t size;
            BuffersPosition begin;
            BuffersPosition end;
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_NALU_BUFFER_H_