// AvcNaluHelper.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcNaluHelper.h"
#include "ppbox/avcodec/avc/AvcNalu.h"

#include <util/buffers/BuffersSize.h>
#include <util/buffers/BuffersCopy.h>

#include <framework/system/BytesOrder.h>

namespace ppbox
{
    namespace avcodec
    {

        AvcNaluHelper::AvcNaluHelper(
            boost::uint8_t nalu_length_size)
            : nalu_length_size_(nalu_length_size)
        {
            nalu_start_code_[0] = 0;
            nalu_start_code_[1] = 0;
            nalu_start_code_[2] = 0;
            nalu_start_code_[3] = 1;
        }

        void AvcNaluHelper::nalus(
            std::vector<NaluBuffer> & nalus)
        {
            return nalus.swap(nalus_);
        }

        static void continue_find_nalu(
            NaluBuffer::FindIterator2 & ibeg, // 记录 0001 开始的位置
            NaluBuffer::FindIterator2 & iter, 
            NaluBuffer::FindIterator2 & iend)
        {
            if (ibeg == iend) {
                return;
            }
            iter.skip_bytes(2);
            while (iter != iend) {
                boost::uint8_t byte = iter->dereference_byte();
                iter.skip_bytes(1);
                if (byte == 0) {
                } else if (byte == 1) {
                    break;
                } else {
                    if (++iter != iend) {
                        ibeg = iter;
                        iter.skip_bytes(2);
                    } else {
                        break;
                    }
                }
            }
        }

        bool AvcNaluHelper::from_stream(
            boost::uint32_t size, 
            buffers_t const & data)
        {
            nalus_.clear();
            NaluBuffer::FindIterator2 ibeg(data, boost::asio::buffer("\0\0", 2));
            NaluBuffer::FindIterator2 iter = ibeg;
            NaluBuffer::FindIterator2 iend;
            continue_find_nalu(ibeg, iter, iend);
            while (ibeg != iend) {
                NaluBuffer::BuffersPosition cur_pos = *iter;
                NaluHeader const nalu_header(cur_pos.dereference_byte());
                if (nalu_header.nal_unit_type == NaluHeader::IDR ||
                    nalu_header.nal_unit_type == NaluHeader::UNIDR) {
                        // 假定 IDR，UNIDR 是最后一个Nalu，不继续搜索，以提高效率
                        nalus_.push_back(NaluBuffer(size - cur_pos.skipped_bytes(), cur_pos, iter.end_position()));
                        break;
                }
                ibeg = ++iter;
                continue_find_nalu(ibeg, iter, iend);
                nalus_.push_back(NaluBuffer(cur_pos, ibeg.position()));
            }
            return true;
        }

        bool AvcNaluHelper::from_packet(
            boost::uint32_t size, 
            buffers_t const & data)
        {
            nalus_.clear();
            NaluBuffer::BuffersPosition position(data.begin(), data.end());
            NaluBuffer::BuffersPosition end(data.end());
            while (!position.at_end()) {
                boost::uint32_t len = 0;
                util::buffers::buffers_copy(
                    boost::asio::buffer((char *)&len + 4 - nalu_length_size_, nalu_length_size_), 
                    util::buffers::Container<boost::asio::const_buffer, NaluBuffer::BufferIterator>(NaluBuffer::BufferIterator(position, end)));
                len = framework::system::BytesOrder::net_to_host_long(len);
                position.increment_bytes(end, nalu_length_size_);
                NaluBuffer::BuffersPosition pos = position;
                position.increment_bytes(end, len);
                nalus_.push_back(NaluBuffer(pos, position));
            }
            return true;
        }

        bool AvcNaluHelper::from_stream(
            buffers_t const & datas)
        {
            return from_stream(util::buffers::buffers_size(datas), datas);
        }

        bool AvcNaluHelper::from_packet(
            buffers_t const & datas)
        {
            return from_packet(0, datas);
        }

        bool AvcNaluHelper::to_stream(
            boost::uint32_t & size, 
            buffers_t & datas)
        {
            for (boost::uint32_t i = 0; i < nalus_.size(); ++i) {
                NaluBuffer const & nalu = nalus_[i];
                NaluHeader nalu_header(nalu.begin.dereference_byte());
                datas.push_back(boost::asio::buffer(nalu_start_code_));
                datas.insert(datas.end(), nalu.buffers_begin(), nalu.buffers_end());
                size += nalu.size + 4;
            }
            return true;
        }

        bool AvcNaluHelper::to_packet(
            boost::uint32_t & size, 
            buffers_t & datas)
        {
            size_t n = 0;
            for (boost::uint32_t i = 0; i < nalus_.size(); ++i) {
                NaluBuffer const & nalu = nalus_[i];
                NaluHeader const nalu_header(nalu.begin.dereference_byte());
                if (nalu_header.nal_unit_type == NaluHeader::UNIDR 
                    || nalu_header.nal_unit_type == NaluHeader::IDR
                    || nalu_header.nal_unit_type == NaluHeader::SEI) {
                        assert(n < sizeof(nalu_length_) / sizeof(nalu_length_[0]));
                        size += nalu.size + nalu_length_size_;
                        nalu_length_[n] = framework::system::BytesOrder::host_to_big_endian_long(nalu.size);
                        datas.push_back(boost::asio::buffer((boost::uint8_t*)&(nalu_length_[n]) + 4 - nalu_length_size_, nalu_length_size_));
                        datas.insert(datas.end(), nalu.buffers_begin(), nalu.buffers_end());
                        ++n;
                }
            }
            return true;
        }

        bool AvcNaluHelper::to_stream(
            buffers_t & data)
        {
            boost::uint32_t size = 0;
            return to_stream(size, data);
        }

        bool AvcNaluHelper::to_packet(
            buffers_t & data)
        {
            boost::uint32_t size = 0;
            return to_packet(size, data);
        }

    } // namespace avcodec
} // namespace ppbox
