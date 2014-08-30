// AvcFrameType.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_FRAME_TYPE_H_
#define _PPBOX_AVCODEC_AVC_AVC_FRAME_TYPE_H_

#include "ppbox/avcodec/nalu/NaluHelper.h"

#include <istream>

namespace ppbox
{
    namespace avcodec
    {

        class AvcFrameType
        {
        public:
            typedef NaluBuffer::ConstBuffers buffers_t;

        public:
            AvcFrameType();

            ~AvcFrameType();

        public:
            // return 0 if not found
            boost::uint32_t handle(
                std::vector<boost::uint8_t> const & data);

            boost::uint32_t handle(
                buffers_t const & datas);

            boost::uint32_t handle(
                std::basic_istream<boost::uint8_t> & is);

        public:
            bool is_sync_frame() const;

        private:
            boost::uint32_t check_offset(
                buffers_t const & datas, 
                boost::uint32_t offset);

            boost::uint32_t check_offset(
                std::basic_istream<boost::uint8_t> & is, 
                boost::uint32_t offset);

            boost::uint32_t get_type(
                boost::uint32_t offset, 
                boost::uint8_t byte);

        private:
            NaluHelper helper_;
            boost::uint32_t offsets_[2];
            boost::uint32_t totals_[2];
            boost::uint32_t hits_[2];
            boost::uint32_t type_;
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_FRAME_TYPE_H_
