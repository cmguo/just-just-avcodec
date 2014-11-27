// FakeTranscoder.h

#ifndef _JUST_AVCODEC_FAKE_TRANSCODER_H_
#define _JUST_AVCODEC_FAKE_TRANSCODER_H_

#include "just/avcodec/Transcoder.h"

#include <framework/system/LogicError.h>

namespace just
{
    namespace avcodec
    {

        class FakeTranscoder
            : public Transcoder
        {
        public:
            virtual void probe(
                boost::uint32_t category, // video, audio ...
                boost::uint32_t input_codec, 
                output_codecs_t & output_codecs) const
            {
            }

        public:
            virtual bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }

            virtual bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }

            virtual bool push(
                eos_t const & eos, 
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }

            virtual bool refresh(
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }

            virtual bool close(
                boost::system::error_code & ec)
            {
                ec = framework::system::logic_error::not_supported;
                return false;
            }
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_FAKE_TRANSCODER_H_
