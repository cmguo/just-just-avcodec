// FFMpegAudioDecoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/ffmpeg/FFMpegAudioDecoder.h"
#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/ffmpeg/FFMpegAudioDecoderImpl.h"

namespace ppbox
{
    namespace avcodec
    {

        FFMpegAudioDecoder::FFMpegAudioDecoder()
            : Transcoder2(StreamType::AUDI)
            , impl_(new FFMpegAudioDecoderImpl())
        {
            for (size_t i = 0; i < FFMpegCodecMap::count(); ++i) {
                register_codec(FFMpegCodecMap::table()[i].type, AudioSubType::PCM);
                register_codec(FFMpegCodecMap::table()[i].type, AudioSubType::FLT);
            }
        }

        FFMpegAudioDecoder::~FFMpegAudioDecoder()
        {
            delete impl_;
        }

        bool FFMpegAudioDecoder::config(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            return impl_->config(config, ec);
        }

        bool FFMpegAudioDecoder::open(
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

        bool FFMpegAudioDecoder::push(
            Sample const & sample, 
            boost::system::error_code & ec)
        {
            return impl_->push(sample, ec);
        }

        bool FFMpegAudioDecoder::push(
            eos_t const & eos, 
            boost::system::error_code & ec)
        {
            return impl_->got_frame > 0;
        }

        bool FFMpegAudioDecoder::pop(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return impl_->pop(sample, ec);
        }

        bool FFMpegAudioDecoder::refresh(
            boost::system::error_code & ec)
        {
            return impl_->refresh(ec);
        }

        bool FFMpegAudioDecoder::close(
            boost::system::error_code & ec)
        {
            return impl_->close(ec);
        }

    } // namespace avcodec
} // namespace ppbox
