// Debuger.h

#ifndef _PPBOX_AVCODEC_DEBUGER_H_
#define _PPBOX_AVCODEC_DEBUGER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <ppbox/common/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Debuger
            : public ppbox::common::ClassFactory<
                Debuger, 
                boost::uint32_t, 
                Debuger *()
            >
        {
        public:
            Debuger()
            {
            }

            virtual ~Debuger()
            {
            }

        public:
            virtual bool reset(
                StreamInfo & info, 
                boost::system::error_code & ec) = 0;

            virtual bool debug(
                Sample & sample, 
                boost::system::error_code & ec) = 0;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_DEBUGER(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVCODEC_DEBUGER_H_
