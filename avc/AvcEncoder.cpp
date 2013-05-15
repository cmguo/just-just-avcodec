// AvcEncoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcEncoder.h"
#include "ppbox/avcodec/avc/AvcEncoderX264Impl.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcEncoder::AvcEncoder()
        {
            impl_ = new x264_impl;
        }

        AvcEncoder::~AvcEncoder()
        {
            delete impl_;
        }

        bool AvcEncoder::open(
            StreamInfo const & input_format, 
            std::map<std::string, std::string> const & config, 
            StreamInfo & output_format, 
            boost::system::error_code & ec)
        {
            return impl_->open(input_format, config, output_format, ec);
        }

        bool AvcEncoder::push(
            Sample const & sample, 
            boost::system::error_code & ec)
        {
            return impl_->push(sample, ec);
        }

        bool AvcEncoder::pop(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            return impl_->pop(sample, ec);
        }

        bool AvcEncoder::close(
            boost::system::error_code & ec)
        {
            return impl_->close(ec);
        }

    } // namespace avcodec
} // namespace ppbox
