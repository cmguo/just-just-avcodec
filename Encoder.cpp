// Encoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Encoder.h"
#include "ppbox/avcodec/Error.h"

namespace ppbox
{
    namespace avcodec
    {

        Encoder::Encoder()
        {
        }

        Encoder::~Encoder()
        {
        }

        boost::system::error_code EncoderTraits::error_not_found()
        {
            return error::encoder_not_support;
        }

    } // namespace avcodec
} // namespace ppbox
