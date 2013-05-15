// AvcodecModule.h

#ifndef _PPBOX_AVCODEC_AVCODEC_MODULE_H_
#define _PPBOX_AVCODEC_AVCODEC_MODULE_H_

#include "ppbox/common/CommonModuleBase.h"

namespace ppbox
{
    namespace avcodec
    {

        class AvcodecModule
            : public ppbox::common::CommonModuleBase<AvcodecModule>
        {
        public:
            AvcodecModule(
                util::daemon::Daemon & daemon);

            ~AvcodecModule();

        public:
            virtual boost::system::error_code startup();

            virtual void shutdown();
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVCODEC_MODULE_H_
