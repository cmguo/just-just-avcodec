// Debuger.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Debuger.h"
#include "ppbox/avcodec/Error.h"

namespace ppbox
{
    namespace avcodec
    {

        Debuger::Debuger()
        {
        }

        Debuger::~Debuger()
        {
        }

        boost::system::error_code DebugerTraits::error_not_found()
        {
            return error::debuger_not_support;
        }

    } // namespace avcodec
} // namespace ppbox
