// Encoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Encoder.h"
#include "ppbox/avcodec/Error.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcEncoder.h"
#include "ppbox/avcodec/aac/AacEncoder.h"

namespace ppbox
{
    namespace avcodec
    {

        boost::system::error_code Encoder::error_not_found()
        {
            return error::encoder_not_support;
        }

        Encoder::Encoder()
        {
        }

        Encoder::~Encoder()
        {
        }

    } // namespace avcodec
} // namespace ppbox
