// Debuger.h

#ifndef _JUST_AVCODEC_DEBUGER_H_
#define _JUST_AVCODEC_DEBUGER_H_

#include "just/avcodec/Base.h"

#include <util/tools/ClassFactory.h>

#include <fstream>

namespace just
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
                boost::system::error_code & ec);

            virtual bool debug(
                Sample & sample, 
                boost::system::error_code & ec);

        protected:
            template <typename ConstBuffers>
            void dump(
                ConstBuffers const & buffers)
            {
                typename ConstBuffers::const_iterator beg = buffers.begin();
                typename ConstBuffers::const_iterator end = buffers.end();
                for (; beg != end; ++beg) {
                    file_.write(
                        boost::asio::buffer_cast<char const *>(*beg), 
                        boost::asio::buffer_size(*beg));
                }
            }

        private:
            std::string dump_file_;
            std::ofstream file_;
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
} // namespace just

#define JUST_REGISTER_DEBUGER(key, cls) UTIL_REGISTER_CLASS(just::avcodec::DebugerFactory, key, cls)

#endif // _JUST_AVCODEC_DEBUGER_H_
