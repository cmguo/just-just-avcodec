// Debuger.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Debuger.h"
#include "ppbox/avcodec/Error.h"

#include <util/tools/ClassRegister.h>
#include "ppbox/avcodec/avc/AvcDebuger.h"
#include "ppbox/avcodec/aac/AacDebuger.h"

namespace ppbox
{
    namespace avcodec
    {

        boost::system::error_code Debuger::error_not_found()
        {
            return error::debuger_not_support;
        }

        Debuger::Debuger()
        {
        }

        Debuger::~Debuger()
        {
        }

    } // namespace avcodec
} // namespace ppbox
