// AacEncoder.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/aac/AacEncoder.h"
#include "just/avcodec/aac/AacFormatType.h"
#ifndef JUST_DISABLE_FDK_AAC
#  include "just/avcodec/aac/AacEncoderFdkImpl.h"
#else
#  include "just/avcodec/FakeTranscoder.h"
#endif

namespace just
{
    namespace avcodec
    {

#ifdef JUST_DISABLE_FDK_AAC
        struct fdk_aac_impl : FakeTranscoder {};
#endif

        AacEncoder::AacEncoder()
            : Transcoder2(StreamType::AUDI)
        {
            impl_ = new fdk_aac_impl;
            register_codec(AudioType::PCM, AudioType::AAC);
        }

        AacEncoder::~AacEncoder()
        {
            delete impl_;
        }

        bool AacEncoder::config(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            return impl_->config(config, ec);
        }

        bool AacEncoder::open(
            StreamInfo const & input_format, 
            StreamInfo & output_format, 
            boost::system::error_code & ec)
        {
            return impl_->open(input_format, output_format, ec);
        }

        bool AacEncoder::push(
            Sample const & sample, 
            boost::system::error_code & ec)
        {
            return impl_->push(sample, ec);
        }

        bool AacEncoder::push(
            eos_t const & eos, 
            boost::system::error_code & ec)
        {
            return impl_->push(eos, ec);
        }

        bool AacEncoder::pop(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return impl_->pop(sample, ec);
        }

        bool AacEncoder::refresh(
            boost::system::error_code & ec)
        {
            return impl_->refresh(ec);
        }

        bool AacEncoder::close(
            boost::system::error_code & ec)
        {
            return impl_->close(ec);
        }

    } // namespace avcodec
} // namespace just
