// Transcoder2.h

#ifndef _PPBOX_AVCODEC_TRANSCODER2_H_
#define _PPBOX_AVCODEC_TRANSCODER2_H_

#include "ppbox/avcodec/Transcoder.h"

namespace ppbox
{
    namespace avcodec
    {

        class Transcoder2
            : public Transcoder
        {
        protected:
            Transcoder2(
                boost::uint32_t category); // video, audio ...

            Transcoder2(
                boost::uint32_t category, 
                boost::uint32_t input_codec, 
                output_codecs_t const & output_codecs);

            Transcoder2(
                boost::uint32_t category, 
                boost::uint32_t input_codec, 
                boost::uint32_t output_codec);

            virtual ~Transcoder2();

        protected:
            virtual void probe(
                boost::uint32_t category, // video, audio ...
                boost::uint32_t input_codec, 
                output_codecs_t & output_codecs) const;

        protected:
            void register_codec(
                boost::uint32_t input_codec, 
                boost::uint32_t output_codec);

            void register_codec(
                boost::uint32_t input_codec, 
                output_codecs_t const & output_codecs);

        private:
            boost::uint32_t category_;
            std::map<boost::uint32_t, output_codecs_t> codecs_;
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_TRANSCODER2_H_
