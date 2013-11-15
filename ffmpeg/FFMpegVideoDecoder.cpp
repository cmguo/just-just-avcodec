// FFMpegVideoDecoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/ffmpeg/FFMpegVideoDecoder.h"
#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/ffmpeg/FFMpegVideoDecoderImpl.h"

namespace ppbox
{
    namespace avcodec
    {

        FFMpegVideoDecoder::FFMpegVideoDecoder()
            : Transcoder2(StreamType::VIDE)
            , impl_(new FFMpegVideoDecoderImpl())
        {
            FFMpegCodec const * table = FFMpegCodecMap::table();
            for (size_t i = 0; i < FFMpegCodecMap::count(); ++i) {
                if (table[i].category == StreamType::VIDE) {
                    register_codec(table[i].type, VideoSubType::I420);
                    register_codec(table[i].type, VideoSubType::RGB8);
                }
            }
        }

        FFMpegVideoDecoder::~FFMpegVideoDecoder()
        {
            delete impl_;
        }

        bool FFMpegVideoDecoder::config(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            return impl_->config(config, ec);
        }

        bool FFMpegVideoDecoder::open(
            StreamInfo const & input_format, 
            StreamInfo & output_format, 
            boost::system::error_code & ec)
        {
            return impl_->open(input_format, output_format, ec);
        }

        static boost::uint8_t const * copy_sample_data(
            Sample const & sample)
        {
            static std::vector<boost::uint8_t> sample_buffer;
            if (sample.data.size() == 1) {
                return boost::asio::buffer_cast<boost::uint8_t const *>(sample.data[0]);
            } else {
                if (sample_buffer.size() < sample.size)
                    sample_buffer.resize(sample.size);
                util::buffers::buffers_copy(boost::asio::buffer(sample_buffer), sample.data);
                return sample_buffer.size() ? &sample_buffer.at(0) : NULL;
            }
        }

        bool FFMpegVideoDecoder::push(
            Sample const & sample, 
            boost::system::error_code & ec)
        {
            return impl_->push(sample, ec);
        }

        bool FFMpegVideoDecoder::push(
            eos_t const & eos, 
            boost::system::error_code & ec)
        {
            return impl_->push(eos, ec);
        }

        bool FFMpegVideoDecoder::pop(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return impl_->pop(sample, ec);
        }

        bool FFMpegVideoDecoder::refresh(
            boost::system::error_code & ec)
        {
            return impl_->refresh(ec);
        }

        bool FFMpegVideoDecoder::close(
            boost::system::error_code & ec)
        {
            return impl_->close(ec);
        }

    } // namespace avcodec
} // namespace ppbox
