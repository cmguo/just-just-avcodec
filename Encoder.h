// Encoder.h

#ifndef _PPBOX_AVCODEC_ENCODER_H_
#define _PPBOX_AVCODEC_ENCODER_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

#include <ppbox/common/ClassFactory.h>

#include <boost/intrusive_ptr.hpp>

namespace ppbox
{
    namespace avcodec
    {

        class Encoder
            : public ppbox::common::ClassFactory<
                Encoder, 
                boost::uint32_t, 
                Encoder *()
            >
        {
        public:
            Encoder()
            {
            }

            virtual ~Encoder()
            {
            }

        public:
            virtual bool open(
                StreamInfo const & input_format, 
                std::map<std::string, std::string> const & config, 
                StreamInfo & output_format, 
                boost::system::error_code & ec) = 0;

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool close(
                boost::system::error_code & ec) = 0;

        private:
            friend void intrusive_ptr_add_ref(
                Encoder const * p)
            {
                ++p->nref_;
            }

            friend void intrusive_ptr_release(
                Encoder const * p)
            {
                if (--p->nref_ == 0) {
                    delete p;
                }
            }

        private:
            mutable size_t nref_;
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_ENCODER(key, cls) PPBOX_REGISTER_CLASS(key, cls)

#endif // _PPBOX_AVCODEC_ENCODER_H_
