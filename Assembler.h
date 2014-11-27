// Assembler.h

#ifndef _JUST_AVCODEC_ASSEMBLER_H_
#define _JUST_AVCODEC_ASSEMBLER_H_

#include "just/avcodec/Base.h"

#include <util/tools/ClassFactory.h>

namespace just
{
    namespace avcodec
    {

        class Assembler
        {
        public:
            Assembler();

            virtual ~Assembler();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec) = 0;
        };

        struct AssemblerTraits
            : util::tools::ClassFactoryTraits
        {
            typedef boost::uint64_t key_type;
            typedef Assembler * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        class AssemblerFactory
            : public util::tools::ClassFactory<AssemblerTraits>
        {
        public:
            static Assembler * create(
                boost::uint32_t codec_type, 
                boost::uint32_t format, 
                boost::system::error_code & ec);
        };

    } // namespace avcodec
} // namespace just

#define JUST_REGISTER_ASSEMBLER(codec_type, format, cls) \
    UTIL_REGISTER_CLASS(just::avcodec::AssemblerFactory, ((boost::uint64_t)codec_type << 32) | format, cls)

#endif // _JUST_AVCODEC_ASSEMBLER_H_
