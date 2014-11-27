// Transcoder2.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/Transcoder2.h"

namespace just
{
    namespace avcodec
    {

        Transcoder2::Transcoder2(
            boost::uint32_t category)
            : category_(category)
        {
        }

        Transcoder2::Transcoder2(
            boost::uint32_t category, 
            boost::uint32_t input_codec, 
            boost::uint32_t output_codec)
            : category_(category)
        {
            register_codec(input_codec, output_codec);
        }

        Transcoder2::Transcoder2(
            boost::uint32_t category, 
            boost::uint32_t input_codec, 
            output_codecs_t const & output_codecs)
            : category_(category)
        {
            register_codec(input_codec, output_codecs);
        }

        Transcoder2::~Transcoder2()
        {
        }

        void Transcoder2::probe(
            boost::uint32_t category, // video, audio ...
            boost::uint32_t input_codec, 
            output_codecs_t & output_codecs) const
        {
            if (category != category_)
                return;
            std::map<boost::uint32_t, output_codecs_t>::const_iterator iter = codecs_.find(input_codec);
            if (iter != codecs_.end()) {
                output_codecs = iter->second;
            }
        }

        void Transcoder2::register_codec(
            boost::uint32_t input_codec, 
            boost::uint32_t output_codec)
        {
            codecs_[input_codec].push_back(output_codec);
        }

        void Transcoder2::register_codec(
            boost::uint32_t input_codec, 
            output_codecs_t const & output_codecs)
        {
            codecs_[input_codec] = output_codecs;
        }

    } // namespace avcodec
} // namespace just
