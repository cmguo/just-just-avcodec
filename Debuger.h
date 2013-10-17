// Debuger.h

#ifndef _PPBOX_AVCODEC_DEBUGER_H_
#define _PPBOX_AVCODEC_DEBUGER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Debuger
        {
        public:
            Debuger();

            virtual ~Debuger();

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool debug(
                Sample & sample, 
                boost::system::error_code & ec) = 0;
        };

        struct DebugerTraits
            : util::tools::ClassFactoryTraits
        {
            typedef boost::uint32_t key_type;
            typedef Debuger * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        typedef util::tools::ClassFactory<DebugerTraits> DebugerFactory;

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_DEBUGER(key, cls) UTIL_REGISTER_CLASS(ppbox::avcodec::DebugerFactory, key, cls)

#endif // _PPBOX_AVCODEC_DEBUGER_H_
