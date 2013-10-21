// AvcEncoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcEncoder.h"
#include "ppbox/avcodec/avc/AvcEncoderX264Impl.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcEncoder::AvcEncoder()
            : Transcoder2(StreamType::VIDE)
        {
            impl_ = new x264_impl;
            register_codec(VideoSubType::I420, VideoSubType::AVC1);
            register_codec(VideoSubType::YV12, VideoSubType::AVC1);
            register_codec(VideoSubType::NV12, VideoSubType::AVC1);
            register_codec(VideoSubType::I422, VideoSubType::AVC1);
            register_codec(VideoSubType::YV16, VideoSubType::AVC1);
            register_codec(VideoSubType::NV16, VideoSubType::AVC1);
            register_codec(VideoSubType::I444, VideoSubType::AVC1);
            register_codec(VideoSubType::YV24, VideoSubType::AVC1);
            register_codec(VideoSubType::BGR8, VideoSubType::AVC1);
            register_codec(VideoSubType::BGRA, VideoSubType::AVC1);
            register_codec(VideoSubType::RGB8, VideoSubType::AVC1);
        }

        AvcEncoder::~AvcEncoder()
        {
            delete impl_;
        }

        bool AvcEncoder::config(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            return impl_->config(config, ec);
        }

        bool AvcEncoder::open(
            StreamInfo const & input_format, 
            StreamInfo & output_format, 
            boost::system::error_code & ec)
        {
            return impl_->open(input_format, output_format, ec);
        }

        bool AvcEncoder::push(
            Sample const & sample, 
            boost::system::error_code & ec)
        {
            return impl_->push(sample, ec);
        }

        bool AvcEncoder::push(
            eos_t const & eos, 
            boost::system::error_code & ec)
        {
            return impl_->push(eos, ec);
        }

        bool AvcEncoder::pop(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return impl_->pop(sample, ec);
        }

        bool AvcEncoder::refresh(
            boost::system::error_code & ec)
        {
            return impl_->refresh(ec);
        }

        bool AvcEncoder::close(
            boost::system::error_code & ec)
        {
            return impl_->close(ec);
        }

    } // namespace avcodec
} // namespace ppbox
