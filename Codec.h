// Codec.h

#ifndef _PPBOX_AVCODEC_CODEC_H_
#define _PPBOX_AVCODEC_CODEC_H_

#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/StreamInfo.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Codec
        {
        public:
            Codec();

            virtual ~Codec();

        public:
            virtual bool finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

        public:
            static bool static_finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec);
        };

        struct CodecTraits
            : util::tools::ClassFactoryTraits
        {
            typedef boost::uint32_t key_type;
            typedef Codec * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        typedef util::tools::ClassFactory<CodecTraits> CodecFactory;

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_CODEC(key, cls) UTIL_REGISTER_CLASS(ppbox::avcodec::CodecFactory, key, cls)

#endif // _PPBOX_AVCODEC_CODEC_H_
