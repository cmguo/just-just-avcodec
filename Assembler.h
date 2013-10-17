// Assembler.h

#ifndef _PPBOX_AVCODEC_ASSEMBLER_H_
#define _PPBOX_AVCODEC_ASSEMBLER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
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
} // namespace ppbox

#define PPBOX_REGISTER_ASSEMBLER(codec_type, format, cls) \
    UTIL_REGISTER_CLASS(ppbox::avcodec::AssemblerFactory, ((boost::uint64_t)codec_type << 32) | format, cls)

#endif // _PPBOX_AVCODEC_ASSEMBLER_H_
