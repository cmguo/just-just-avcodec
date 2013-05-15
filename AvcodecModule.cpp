// AvcodecModule.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/AvcodecModule.h"
#include "ppbox/avcodec/CodecTypes.h"

namespace ppbox
{
    namespace avcodec
    {

        AvcodecModule::AvcodecModule(
            util::daemon::Daemon & daemon)
            : ppbox::common::CommonModuleBase<AvcodecModule>(daemon, "AvcodecModule")
        {
        }

        AvcodecModule::~AvcodecModule()
        {
        }

        boost::system::error_code AvcodecModule::startup()
        {
            boost::system::error_code ec;
            return ec;
        }

        void AvcodecModule::shutdown()
        {
        }

    } // namespace avcodec
} // namespace ppbox
