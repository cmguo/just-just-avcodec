// HevcByteStreamAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/hevc/HevcByteStreamAssembler.h"
#include "ppbox/avcodec/hevc/HevcEnum.h"
#include "ppbox/avcodec/hevc/HevcConfigHelper.h"
#include "ppbox/avcodec/hevc/HevcNaluHeader.h"
#include "ppbox/avcodec/nalu/NaluHelper.h"

namespace ppbox
{
    namespace avcodec
    {

        HevcByteStreamAssembler::HevcByteStreamAssembler()
        {
        }

        HevcByteStreamAssembler::~HevcByteStreamAssembler()
        {
        }

        bool HevcByteStreamAssembler::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            HevcConfigHelper const & config = *(HevcConfigHelper const *)info.context;
            config.to_es_data(info.format_data);
            info.format_type = HevcFormatType::byte_stream;

            // access unit delimiter
            boost::uint8_t nalu_start_code[] = {0, 0, 0, 1};
            access_unit_delimiter_.assign(nalu_start_code, nalu_start_code + 4);
            HevcNaluHeader nalu_header;
            nalu_header.nal_unit_type = HevcNaluType::AUD_NUT;
            nalu_header.nuh_temporal_id_plus1 = 1;
            access_unit_delimiter_.push_back(nalu_header.byte1());
            access_unit_delimiter_.push_back(nalu_header.byte2());
            access_unit_delimiter_.push_back(0xE0);

            // sequence
            sequence_header_ = info.format_data;

            init_ = false;

            return true;
        }

        bool HevcByteStreamAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            NaluHelper & helper = *(NaluHelper *)sample.context;

            if (!init_) {
                init_ = true;
                bool need_aud = true;
                bool need_sequence = true;
                std::vector<ppbox::avcodec::NaluBuffer> const & nalus = helper.nalus();
                for (boost::uint32_t i = 0; i < nalus.size(); ++i) {
                    NaluBuffer const & nalu = nalus[i];
                    HevcNaluHeader nalu_header(nalu.begin.dereference_byte());
                    if (nalu_header.nal_unit_type == HevcNaluType::AUD_NUT) {
                        need_aud = false;
                    }
                    if (nalu_header.nal_unit_type == HevcNaluType::VPS_NUT) {
                        need_sequence = false;
                    }
                }
                if (need_aud) {
                    std::cout << "need_aud" << std::endl;
                } else {
                    access_unit_delimiter_.clear();
                }
                if (need_sequence) {
                    std::cout << "need_sequence" << std::endl;
                } else {
                    sequence_header_.clear();
                }
            }

            sample.size = 0;
            NaluBuffer::ConstBuffers data;

            if (sample.flags & sample.f_sync) { 
                data.push_front(boost::asio::buffer(sequence_header_));
                sample.size += sequence_header_.size();
            }

            data.push_front(boost::asio::buffer(access_unit_delimiter_));
            sample.size += access_unit_delimiter_.size();

            bool b = helper.to_stream(sample.size, data);
            sample.data.swap(data);

            sample.context = NULL;
            return b;
        }

    } // namespace avcodec
} // namespace ppbox
