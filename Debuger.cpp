// Debuger.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/Debuger.h"
#include "just/avcodec/Error.h"

#include <just/avbase/FourCC.h>

namespace just
{
    namespace avcodec
    {

        Debuger::Debuger()
        {
        }

        Debuger::~Debuger()
        {
        }

        bool Debuger::reset(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            dump_file_.append(1, (char)info.index + '0');
            dump_file_.append(1, '.');
            dump_file_ += just::avbase::FourCC::to_string(info.sub_type);
            file_.open(dump_file_.c_str(), std::ios::binary | std::ios::trunc);
            return true;
        }

        bool Debuger::debug(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            dump(sample.data);
            return true;
        }

        boost::system::error_code DebugerTraits::error_not_found()
        {
            return error::debuger_not_support;
        }

    } // namespace avcodec
} // namespace just
