// Splitter.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/Splitter.h"
#include "just/avcodec/Error.h"

namespace just
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
} // namespace just
