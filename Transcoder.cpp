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
            boost::uint32_t output_codec, 
            transcoder_chain_t & transcoder_chain, 
            boost::system::error_code & ec)
        {
            struct select_transcoder_t
            {
                size_t index;
                size_t index2;
                std::vector<boost::uint32_t> output_codecs;
            };
            std::vector<transcoder_t2> transcoders;
            std::vector<select_transcoder_t> select_transcoders;
            for (orders_t::const_iterator iter = orders().begin(); iter != orders().end(); ++iter) {
                transcoders.push_back(iter->second);
            }
            select_transcoder_t fake;
            fake.index = (size_t)-1;
            fake.index2 = (size_t)-1;
            fake.output_codecs.push_back(input_codec);
            select_transcoders.push_back(fake);
            while (!select_transcoders.empty()) {
                select_transcoder_t & s = select_transcoders.back();
                std::vector<boost::uint32_t>::const_iterator iter = 
                    std::find(s.output_codecs.begin(), s.output_codecs.end(), output_codec);
                if (iter != s.output_codecs.end()) {
                    s.index2 = iter - s.output_codecs.begin();
                    break;
                }
                if (++s.index2 >= s.output_codecs.size()) {
                    if (++s.index < transcoders.size()) {
                        s.index2 = (size_t)-1;
                        transcoders[s.index].second->probe(category, input_codec, s.output_codecs);
                    } else {
                        select_transcoders.pop_back();
                    }
                    continue;
                }
                select_transcoder_t s1;
                s1.index = (size_t)-1;
                s1.index2 = (size_t)-1;
                select_transcoders.push_back(s1);
            }
            if (select_transcoders.empty()) {
                return false;
            }
            for (size_t i = 1; i < select_transcoders.size(); ++i) {
                transcoder_t t;
                select_transcoder_t & s = select_transcoders[i];
                t.key = transcoders[s.index].first;
                t.codec = s.output_codecs[s.index2];
                t.transcoder = factory_type::create(t.key, ec);
                transcoder_chain.push_back(t);
            }
            return true;
        }

        TranscoderFactory::orders_t & TranscoderFactory::orders()
        {
            static orders_t s_orders;
            return s_orders;
        }

    } // namespace avcodec
} // namespace ppbox
