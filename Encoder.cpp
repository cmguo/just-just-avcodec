// Encoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Encoder.h"

#include <ppbox/common/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcEncoder.h"

namespace ppbox
{
    namespace avcodec
    {

        Encoder::eos_t::eos_t() {} // this can make linker add this cpp

        Encoder::Encoder()
        {
        }

        Encoder::~Encoder()
        {
        }

    } // namespace avcodec
} // namespace ppbox
