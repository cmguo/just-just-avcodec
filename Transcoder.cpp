// Transcoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/Transcoder.h"
#include "ppbox/avcodec/Error.h"

namespace ppbox
{
    namespace avcodec
    {

        Transcoder::Transcoder()
        {
        }

        Transcoder::~Transcoder()
        {
        }

        boost::system::error_code TranscoderTraits::error_not_found()
        {
            return error::encoder_not_support;
        }

        void TranscoderFactory::register_creator(
            std::pair<key_type, boost::uint32_t> const & key_order, 
            creator_type creator)
        {
            factory_type::register_creator(key_order.first, creator);
            transcoder_t2 transcoder = std::make_pair(key_order.first, creator());
            orders().insert(std::make_pair(key_order.second, transcoder));
        }


        bool TranscoderFactory::create_transcodes(
            boost::uint32_t category, 
            boost::uint32_t input_codec, 
            std::vector<boost::uint32_t> const & output_codecs, 
            transcoder_chain_t & transcoder_chain, 
            size_t max_length, 
            boost::system::error_code & ec)
        {
            if (max_length > orders().size())
                max_length = orders().size();

            for (size_t i = 1; i < max_length; ++i) {
                if (create_transcodes(
                    category, 
                    input_codec, 
                    output_codecs, 
                    transcoder_chain, 
                    max_length)) {
                        for (size_t j = 0; j < transcoder_chain.size(); ++j) {
                            transcoder_chain[j].transcoder = factory_type::create(transcoder_chain[j].key, ec);
                        }
                        return true;
                }
            }

            ec = error::codec_not_support;
            return false;
        }

        struct select_transcoder_t
        {
            size_t index;
            size_t output_index;
            std::vector<boost::uint32_t> output_codecs;
        };

        bool TranscoderFactory::create_transcodes(
            boost::uint32_t category, 
            boost::uint32_t input_codec, 
            std::vector<boost::uint32_t> const & output_codecs, 
            transcoder_chain_t & transcoder_chain, 
            size_t max_length)
        {
            std::vector<transcoder_t2> transcoders;
            transcoders.reserve(orders().size());
            for (orders_t::const_iterator iter = orders().begin(); iter != orders().end(); ++iter) {
                transcoders.push_back(iter->second);
            }
            std::vector<bool> used_transcoders;
            used_transcoders.resize(orders().size(), false);

            select_transcoder_t fake;
            fake.index = (size_t)-1;
            fake.output_index = 0;
            fake.output_codecs.push_back(input_codec);
            std::vector<select_transcoder_t> select_transcoders;
            select_transcoders.push_back(fake);

            fake.index = (size_t)-1;
            fake.output_index = (size_t)-1;
            fake.output_codecs.clear();
            select_transcoders.push_back(fake);

            while (select_transcoders.size() > 1) {
                select_transcoder_t & s = select_transcoders.back();
                select_transcoder_t & s1 = select_transcoders[select_transcoders.size() - 2];

                if (++s.index >= transcoders.size()) {
                    used_transcoders[s1.index] = false;
                    select_transcoders.pop_back();
                    continue;
                }

                if (used_transcoders[s.index]) {
                    continue;
                }

                s1.output_index = join_transcoder(
                    category, 
                    s1.output_codecs, 
                    transcoders[s.index].second, 
                    s.output_codecs);

                if (s.output_codecs.empty()) {
                    continue;
                }

                std::vector<boost::uint32_t>::const_iterator iter = std::find_first_of(
                    s.output_codecs.begin(), s.output_codecs.end(), 
                    output_codecs.begin(), output_codecs.end());
                if (iter != s.output_codecs.end()) {
                    s.output_index = iter - s.output_codecs.begin();
                    break;
                }

                if (select_transcoders.size() <= max_length) {
                    used_transcoders[s.index] = true;
                    select_transcoder_t s2;
                    s2.index = (size_t)-1;
                    s2.output_index = (size_t)-1;
                    select_transcoders.push_back(s2);
                }
            }

            if (select_transcoders.size() == 1) {
                return false;
            }

            for (size_t i = 1; i < select_transcoders.size(); ++i) {
                transcoder_t t;
                select_transcoder_t & s = select_transcoders[i];
                t.key = transcoders[s.index].first;
                t.codec = s.output_codecs[s.output_index];
                transcoder_chain.push_back(t);
            }
            return true;
        }

        size_t TranscoderFactory::join_transcoder(
            boost::uint32_t category, 
            std::vector<boost::uint32_t> const & input_codecs, 
            Transcoder * transcoder, 
            std::vector<boost::uint32_t> & output_codecs)
        {
            for (size_t i = 0; i < input_codecs.size(); ++i) {
                transcoder->probe(category, input_codecs[i], output_codecs);
                if (output_codecs.empty())
                    continue;
                return i;
            }
            return (size_t)-1;
        }

        TranscoderFactory::orders_t & TranscoderFactory::orders()
        {
            static orders_t s_orders;
            return s_orders;
        }

    } // namespace avcodec
} // namespace ppbox
