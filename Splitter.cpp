// Splitter.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Splitter.h"
#include "ppbox/avcodec/Error.h"

namespace ppbox
{
    namespace avcodec
    {

        Splitter::Splitter()
        {
        }

        Splitter::~Splitter()
        {
        }

        boost::system::error_code SplitterTraits::error_not_found()
        {
            return error::splitter_not_support;
        }

        Splitter * SplitterFactory::create(
            boost::uint32_t codec_type, 
            boost::uint32_t format, 
            boost::system::error_code & ec)
        {
            return factory_type::create(((boost::uint64_t)codec_type << 32) | format, ec);
        }

    } // namespace avcodec
} // namespace ppbox
