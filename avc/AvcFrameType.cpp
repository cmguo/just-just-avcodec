// AvcFrameType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcFrameType.h"
#include "ppbox/avcodec/avc/AvcEnum.h"
#include "ppbox/avcodec/avc/AvcNaluHeader.h"

#include <util/buffers/BuffersSize.h>
#include <util/buffers/BuffersCopy.h>

#include <framework/system/BytesOrder.h>

namespace ppbox
{
    namespace avcodec
    {

        AvcFrameType::AvcFrameType()
            : helper_(AvcNaluType::is_access_end)
            , type_(0)
        {
            offsets_[0] = offsets_[1] = 0;
            totals_[0] = totals_[1] = 0;
            hits_[0] = hits_[1] = 0;
        }

        AvcFrameType::~AvcFrameType()
        {
        }

        boost::uint32_t AvcFrameType::handle(
            std::vector<boost::uint8_t> const & data)
        {
            buffers_t datas;
            datas.push_back(boost::asio::buffer(data));
            return handle(datas);
        }

        boost::uint32_t AvcFrameType::handle(
            buffers_t const & datas)
        {
            type_ = 0;
            if (offsets_[1] && check_offset(datas, offsets_[1])) {
                ++hits_[1];
                return type_;
            }
            if (offsets_[0] && check_offset(datas, offsets_[0])) {
                ++hits_[0];
                return type_;
            }
            helper_.from_stream(datas);
            if (helper_.nalus().size() > 0) {
                NaluBuffer const & nalu = helper_.nalus().back();
                return get_type(nalu.begin.skipped_bytes(), nalu.begin.dereference_byte());
            } else {
                return 0;
            }
        }

        boost::uint32_t AvcFrameType::handle(
            std::basic_istream<boost::uint8_t> & is)
        {
            type_ = 0;
            if (offsets_[1] && check_offset(is, offsets_[1])) {
                ++hits_[1];
                return type_;
            }
            if (offsets_[0] && check_offset(is, offsets_[0])) {
                ++hits_[0];
                return type_;
            }
            std::vector<boost::uint8_t> data;
            data.reserve(1024);
            data.resize(64);
            is.seekg(0);
            is.read(&data[0], data.size());
            data.resize(is.gcount());
            boost::uint32_t skip = 0;
            while (true) {
                {
                    buffers_t datas;
                    datas.push_back(boost::asio::buffer(data) + skip);
                    helper_.from_stream(datas);
                }
                if (helper_.nalus().size() > 0) {
                    NaluBuffer const & nalu = helper_.nalus().back();
                    if (get_type(nalu.begin.skipped_bytes(), nalu.begin.dereference_byte()))
                        return type_;
                    skip = nalu.begin.skipped_bytes();
                }
                if (is.eof())
                    return 0;
                boost::uint32_t old_size = data.size();
                data.resize(old_size + 32);
                is.read(&data[old_size], 32);
                data.resize(old_size + is.gcount());
            }
        }

        bool AvcFrameType::is_sync_frame() const
        {
            return type_ == AvcNaluType::IDR;
        }

        boost::uint32_t AvcFrameType::check_offset(
            buffers_t const & datas, 
            boost::uint32_t offset)
        {
            union {
                boost::uint8_t data[4];
                boost::uint32_t __uint32;
            };
            boost::uint8_t * p = data;
            boost::uint32_t left = 4;
            offset -= 3;
            for (size_t i = 0; i < datas.size() && left; ++i) {
                boost::uint32_t size = boost::asio::buffer_size(datas[i]);
                if (offset < size) {
                    boost::uint8_t const * q = boost::asio::buffer_cast<boost::uint8_t const *>(datas[i]);
                    if (size > left) {
                        size = left;
                    }
                    memcpy(p, q, size);
                    p += size;
                    left = size;
                    offset = 0;
                } else {
                    offset -= size;
                }
            }
            offset += 3;
#ifdef BOOST_BIG_ENDIAN
            if (left == 0 && (__uint32 & 0xffffff00) == 0x00000100) {
#else
            if (left == 0 && (__uint32 & 0x00ffffff) == 0x00010000) {
#endif
                return get_type(offset, data[3]);
            } else {
                return 0;
            }
        }

        boost::uint32_t AvcFrameType::check_offset(
            std::basic_istream<boost::uint8_t> & is, 
            boost::uint32_t offset)
        {
            union {
                boost::uint8_t data[4];
                boost::uint32_t __uint32;
            };
            is.seekg(offset - 3);
            is.read(data, 4);
            boost::uint32_t left = 4 - is.gcount();
            is.clear();
#ifdef BOOST_BIG_ENDIAN
            if (left == 0 && (__uint32 & 0xffffff00) == 0x00000100) {
#else
            if (left == 0 && (__uint32 & 0x00ffffff) == 0x00010000) {
#endif
                return get_type(offset, data[3]);
            }
            return 0;
        }

        boost::uint32_t AvcFrameType::get_type(
            boost::uint32_t offset, 
            boost::uint8_t byte)
        {
            AvcNaluHeader header(byte);
            if (header.nal_unit_type == AvcNaluType::UNIDR) {
                offsets_[1] = offset;
                ++totals_[1];
            } else if (header.nal_unit_type == AvcNaluType::IDR) {
                offsets_[0] = offset;
                ++totals_[0];
            } else {
                header.nal_unit_type = 0;
            }
            return (type_ = header.nal_unit_type);
        }

    } // namespace avcodec
} // namespace ppbox
