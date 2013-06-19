// Assembler.h

#ifndef _PPBOX_AVCODEC_ASSEMBLER_H_
#define _PPBOX_AVCODEC_ASSEMBLER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Assembler
            : public ppbox::common::ClassFactory<
                Assembler, 
                boost::uint64_t, 
                Assembler *()
            >
        {
        public:
            Assembler();

            virtual ~Assembler();

        public:
            static Assembler * create(
                boost::uint32_t codec_type, 
                boost::uint32_t format);

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool assemble(
                Sample & sample, 
                boost::system::error_code & ec) = 0;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_ASSEMBLER(codec_type, format, cls) PPBOX_REGISTER_CLASS(((boost::uint64_t)codec_type << 32) | format, cls)

#endif // _PPBOX_AVCODEC_ASSEMBLER_H_
