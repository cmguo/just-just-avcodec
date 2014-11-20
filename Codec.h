// Codec.h

#ifndef _PPBOX_AVCODEC_CODEC_H_
#define _PPBOX_AVCODEC_CODEC_H_

#include "ppbox/avcodec/Base.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {


        class CodecFactory;

        class Codec
        {
        public:
            Codec();

            virtual ~Codec();

        public:
           Assembler * create_assembler(
               boost::uint32_t format_type, 
            boost::system::error_code & ec);

           Splitter * create_splitter(
               boost::uint32_t format_type, 
            boost::system::error_code & ec);

           Debuger * create_debuger(
            boost::system::error_code & ec);

        public:
            virtual bool finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

        public:
            static bool static_finish_stream_info(
                StreamInfo & info, 
                boost::system::error_code & ec);

        private:
            boost::uint32_t codec_type_;
            friend class CodecFactory;
        };

        struct CodecTraits
            : util::tools::ClassFactoryTraits
        {
            typedef boost::uint32_t key_type;
            typedef Codec * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        class CodecFactory
            : public util::tools::ClassFactory<CodecTraits> 
        {
        public:
            static Codec * create(
                boost::uint32_t codec_type, 
                boost::system::error_code & ec);
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_CODEC(key, cls) UTIL_REGISTER_CLASS(ppbox::avcodec::CodecFactory, key, cls)

#endif // _PPBOX_AVCODEC_CODEC_H_
