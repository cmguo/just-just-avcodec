// FFMpegAudioDecoder.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/ffmpeg/FFMpegAudioDecoder.h"
#include "just/avcodec/AudioType.h"
#include "just/avcodec/ffmpeg/FFMpegAudioDecoderImpl.h"

namespace just
{
    namespace avcodec
    {

        FFMpegAudioDecoder::FFMpegAudioDecoder()
            : Transcoder2(StreamType::AUDI)
            , impl_(new FFMpegAudioDecoderImpl())
        {
            FFMpegCodec const * table = FFMpegCodecMap::table();
            for (size_t i = 0; i < FFMpegCodecMap::count(); ++i) {
                if (table[i].category == StreamType::AUDI) {
                    register_codec(table[i].type, AudioType::PCM);
                    register_codec(table[i].type, AudioType::FLT);
                }
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
            return impl_->push(eos, ec);
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
} // namespace just
