// NaluHelper.h

#ifndef _JUST_AVCODEC_NALU_NALU_HELPER_H_
#define _JUST_AVCODEC_NALU_NALU_HELPER_H_

#include "just/avcodec/nalu/NaluBuffer.h"

namespace just
{
    namespace avcodec
    {

        class NaluHelper
        {
        public:
            typedef NaluBuffer::ConstBuffers buffers_t;

        public:
            NaluHelper(
                bool (*is_access_end)(boost::uint8_t) = 0, 
                boost::uint8_t nalu_length_size = 4);

        public:
            boost::uint8_t nalu_length_size() const
            {
                return nalu_length_size_;
            }

            void nalu_length_size(
                boost::uint8_t v)
            {
                nalu_length_size_ = v;
            }

            std::vector<NaluBuffer> & nalus()
            {
                return nalus_;
            }

            void nalus(
                std::vector<NaluBuffer> & nalus);

        public:
            bool from_stream(
                boost::uint32_t size, 
                buffers_t const & datas);

            bool from_packet(
                boost::uint32_t size, 
                buffers_t const & datas);

            bool from_stream(
                buffers_t const & datas);

            bool from_packet(
                buffers_t const & datas);

        public:
            bool to_stream(
                boost::uint32_t & size, 
                buffers_t & data);

            bool to_packet(
                boost::uint32_t & size, 
                buffers_t & data);

            bool to_stream(
                buffers_t & data);

            bool to_packet(
                buffers_t & data);

        private:
            bool (*is_access_end_)(boost::uint8_t);
            boost::uint8_t nalu_length_size_;
            std::vector<NaluBuffer> nalus_;
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_NALU_NALU_HELPER_H_
