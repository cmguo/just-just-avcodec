// AvcByteStreamAssembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcByteStreamAssembler.h"
#include "ppbox/avcodec/avc/AvcNalu.h"
#include "ppbox/avcodec/avc/AvcConfigHelper.h"
#include "ppbox/avcodec/avc/AvcNaluHelper.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcByteStreamAssembler::AvcByteStreamAssembler()
        {
        }

        AvcByteStreamAssembler::~AvcByteStreamAssembler()
        {
        }

        bool AvcByteStreamAssembler::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            AvcConfigHelper const & config = *(AvcConfigHelper const *)info.context;
            config.to_es_data(info.format_data);
            info.format_type = AvcFormatType::byte_stream;

            // access unit delimiter
            boost::uint8_t nalu_start_code[] = {0, 0, 0, 1};
            access_unit_delimiter_.assign(nalu_start_code, nalu_start_code + 4);
            access_unit_delimiter_.push_back(9);
            access_unit_delimiter_.push_back(0xF0);
            // sps
            sps_pps_ = info.format_data;

            init_ = false;

            return true;
        }

        bool AvcByteStreamAssembler::assemble(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            AvcNaluHelper & helper = *(AvcNaluHelper *)sample.context;

            if (!init_) {
                init_ = true;
                bool need_aud = true;
                bool need_sps_pps = true;
                std::vector<ppbox::avcodec::NaluBuffer> const & nalus = helper.nalus();
                for (boost::uint32_t i = 0; i < nalus.size(); ++i) {
                    NaluBuffer const & nalu = nalus[i];
                    NaluHeader nalu_header(nalu.begin.dereference_byte());
                    if (nalu_header.nal_unit_type == NaluHeader::AccessUnitDelimiter) {
                        need_aud = false;
                    }
                    if (nalu_header.nal_unit_type == NaluHeader::SPS) {
                        need_sps_pps = false;
                    }
                }
                if (need_aud) {
                    std::cout << "need_aud" << std::endl;
                } else {
                    access_unit_delimiter_.clear();
                }
                if (need_sps_pps) {
                    std::cout << "need_sps_pps" << std::endl;
                } else {
                    sps_pps_.clear();
                }
            }

            sample.size = 0;
            NaluBuffer::ConstBuffers data;

            if (sample.flags & sample.f_sync) { 
                data.push_front(boost::asio::buffer(sps_pps_));
                sample.size += sps_pps_.size();
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
