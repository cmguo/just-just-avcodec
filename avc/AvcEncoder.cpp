// AvcEncoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcEncoder.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"
#ifndef PPBOX_DISABLE_X264
#  include "ppbox/avcodec/avc/AvcEncoderX264Impl.h"
#else
#  include "ppbox/avcodec/FakeTranscoder.h"
#endif

namespace ppbox
{
    namespace avcodec
    {

#ifdef PPBOX_DISABLE_X264
        struct x264_impl : FakeTranscoder {};
#endif

        AvcEncoder::AvcEncoder()
            : Transcoder2(StreamType::VIDE)
        {
            impl_ = new x264_impl;
            register_codec(VideoType::I420, VideoType::AVC);
            register_codec(VideoType::YV12, VideoType::AVC);
            register_codec(VideoType::NV12, VideoType::AVC);
            register_codec(VideoType::I422, VideoType::AVC);
            register_codec(VideoType::YV16, VideoType::AVC);
            register_codec(VideoType::NV16, VideoType::AVC);
            register_codec(VideoType::I444, VideoType::AVC);
            register_codec(VideoType::YV24, VideoType::AVC);
            register_codec(VideoType::BGR8, VideoType::AVC);
            register_codec(VideoType::BGRA, VideoType::AVC);
            register_codec(VideoType::RGB8, VideoType::AVC);
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
