// Assembler.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Assembler.h"
#include "ppbox/avcodec/Error.h"

namespace ppbox
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
} // namespace ppbox
