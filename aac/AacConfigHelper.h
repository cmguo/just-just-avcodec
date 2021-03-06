// AacConfigHelper.h

#ifndef _JUST_AVCODEC_AAC_AAC_CONFIG_HELPER_H_
#define _JUST_AVCODEC_AAC_AAC_CONFIG_HELPER_H_

namespace just
{
    namespace avbase
    {
        struct AudioInfo;
    }

    namespace avcodec
    {

        struct AacConfig;

        class AacConfigHelper
        {
        public:
            AacConfigHelper();

            AacConfigHelper(
                boost::uint8_t const * buf, 
                boost::uint32_t size);

            AacConfigHelper(
                std::vector<boost::uint8_t> const & buf);

            ~AacConfigHelper();

        public:
            AacConfig const & data() const
            {
                return *data_;
            }

        public:
            void from_data(
                std::vector<boost::uint8_t> const & buf);

            void to_data(
                std::vector<boost::uint8_t> & buf) const;

            void from_adts_data(
                std::vector<boost::uint8_t> const & buf);

            void to_adts_data(
                boost::uint32_t frame_size, 
                std::vector<boost::uint8_t> & buf) const;

        public:
            boost::uint32_t get_object_type() const;

            boost::uint32_t get_frequency() const;

            boost::uint32_t get_channel_count() const;

            boost::uint32_t get_extension_object_type() const;

            boost::uint32_t get_extension_frequency() const;

            bool sbr_present() const;

            bool ps_present() const;

        public:
            void set_object_type(
                boost::uint32_t object_type);

            void set_frequency(
                boost::uint32_t frequency);

            void set_channel_count(
                boost::uint32_t channel_count);

        public:
            bool ready() const;

            void get_format(
                just::avbase::AudioInfo & info) const;

        private:
            AacConfig * data_;

        private:
            static boost::uint32_t frequency_table[];
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_AAC_AAC_CONFIG_HELPER_H_
