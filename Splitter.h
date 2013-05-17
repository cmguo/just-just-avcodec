// Splitter.h

#ifndef _PPBOX_AVCODEC_SPLITTER_H_
#define _PPBOX_AVCODEC_SPLITTER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Splitter
            : public ppbox::common::ClassFactory<
                Splitter, 
                boost::uint64_t, 
                Splitter *()
            >
        {
        public:
            Splitter();

            virtual ~Splitter();

        public:
            static Splitter * create(
                boost::uint32_t codec_type, 
                boost::uint32_t format);

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool split(
                Sample & sample, 
                boost::system::error_code & ec) = 0;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_SPLITTER(codec, format, cls) PPBOX_REGISTER_CLASS(((boost::uint64_t)codec << 32) | format, cls)

#endif // _PPBOX_AVCODEC_SPLITTER_H_
