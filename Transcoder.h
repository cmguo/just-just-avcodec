// Transcoder.h

#ifndef _PPBOX_AVCODEC_TRANSCODER_H_
#define _PPBOX_AVCODEC_TRANSCODER_H_

#include "ppbox/avcodec/Base.h"

#include <util/tools/ClassFactory.h>

namespace ppbox
{
    namespace avcodec
    {

        class Transcoder
        {
        public:
            struct eos_t { };

            static eos_t eos()
            {
                return eos_t();
            }

        public:
            Transcoder();

            virtual ~Transcoder();

        public:
            typedef std::vector<boost::uint32_t> output_codecs_t;

            virtual void probe(
                boost::uint32_t category, // video, audio ...
                boost::uint32_t input_codec, 
                output_codecs_t & output_codecs) const = 0;

        public:
            virtual bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec) = 0;

            virtual bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec) = 0;

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool push(
                eos_t const & eos, 
                boost::system::error_code & ec) = 0;

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec) = 0;

            virtual bool refresh(
                boost::system::error_code & ec) = 0;

            virtual bool close(
                boost::system::error_code & ec) = 0;

        protected:
            static boost::uint32_t const SCOPE_MAX = 100;
        };

        struct TranscoderTraits
            : util::tools::ClassFactoryTraits
        {
            typedef std::string key_type;
            typedef Transcoder * (create_proto)();

            static boost::system::error_code error_not_found();
        };

        class TranscoderFactory
            : public util::tools::ClassFactory<TranscoderTraits>
        {
        public:
            template <typename Transcoder>
            static void register_class(
                std::pair<key_type, boost::uint32_t> const & key_order)
            {
                factory_type::register_class<Transcoder>(key_order.first);
                transcoder_t2 transcoder = std::make_pair(key_order.first, new Transcoder());
                orders().insert(std::make_pair(key_order.second, transcoder));
            }

        public:
            struct transcoder_t
            {
                key_type key;
                boost::uint32_t codec;
                Transcoder * transcoder;
            };

            typedef std::vector<transcoder_t> transcoder_chain_t;

            static bool create_transcodes(
                boost::uint32_t category, // video, audio ...
                boost::uint32_t input_codec, 
                std::vector<boost::uint32_t> const & output_codecs, 
                transcoder_chain_t & transcoder_chain, 
                size_t max_length, 
                boost::system::error_code & ec);

        private:
            static bool create_transcodes(
                boost::uint32_t category, // video, audio ...
                boost::uint32_t input_codec, 
                std::vector<boost::uint32_t> const & output_codecs, 
                transcoder_chain_t & transcoder_chain, 
                size_t max_length);

            static size_t join_transcoder(
                boost::uint32_t category, // video, audio ...
                std::vector<boost::uint32_t> const & input_codecs, 
                Transcoder * transcoder, 
                std::vector<boost::uint32_t> & output_codecs);

        private:
            typedef std::pair<std::string, ppbox::avcodec::Transcoder *> transcoder_t2;
            typedef std::multimap<boost::uint32_t, transcoder_t2> orders_t;

            static orders_t & orders();
        };

    } // namespace avcodec
} // namespace ppbox

#define PPBOX_REGISTER_TRANSCODER(key, order, cls) \
    UTIL_REGISTER_CLASS(ppbox::avcodec::TranscoderFactory, std::make_pair(key, order), cls)

#endif // _PPBOX_AVCODEC_TRANSCODER_H_
