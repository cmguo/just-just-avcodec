// Assembler.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/Assembler.h"
#include "just/avcodec/Error.h"

namespace just
{
    namespace avcodec
    {

        Assembler::Assembler()
        {
        }

        Assembler::~Assembler()
        {
        }

        boost::system::error_code AssemblerTraits::error_not_found()
        {
            return error::assembler_not_support;
        }

        Assembler * AssemblerFactory::create(
            boost::uint32_t codec_type, 
            boost::uint32_t format, 
            boost::system::error_code & ec)
        {
            return factory_type::create(((boost::uint64_t)codec_type << 32) | format, ec);
        }

    } // namespace avcodec
} // namespace just
