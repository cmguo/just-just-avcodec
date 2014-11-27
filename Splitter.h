// Splitter.h

#ifndef _JUST_AVCODEC_SPLITTER_H_
#define _JUST_AVCODEC_SPLITTER_H_

#include "just/avcodec/Base.h"

#include <util/tools/ClassFactory.h>

namespace just
{
    namespace avcodec
    {

        class Splitter
        {
        public:
            Splitter();

            virtual ~Splitter();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool split(
                Sample & sample, 
                boost::system::error_code & ec) = 0;
        };

        struct SplitterTraits
            : util::tools::ClassFactoryTraits
        {
            typedef boost::uint64_t key_type;
            typedef Splitter * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        class SplitterFactory
            : public util::tools::ClassFactory<SplitterTraits>
        {
        public:
            static Splitter * create(
                boost::uint32_t codec_type, 
                boost::uint32_t format, 
                boost::system::error_code & ec);
        };

    } // namespace avcodec
} // namespace just

#define JUST_REGISTER_SPLITTER(codec_type, format, cls) \
    UTIL_REGISTER_CLASS(just::avcodec::SplitterFactory, ((boost::uint64_t)codec_type << 32) | format, cls)

#endif // _JUST_AVCODEC_SPLITTER_H_
