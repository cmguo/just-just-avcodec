// MpaConfigHelper.h

#ifndef _PPBOX_AVCODEC_MPA_MPA_CONFIG_HELPER_H_
#define _PPBOX_AVCODEC_MPA_MPA_CONFIG_HELPER_H_

namespace ppbox
{
    namespace avbase
    {
        struct AudioInfo;
    }

    namespace avcodec
    {

        struct MpaConfig;

        class MpaConfigHelper
        {
        public:
            enum VersionEnum
            {
                //v2_5, 
                //v_reserved, 
                v2, 
                v1
            };

            enum LayerEnum
            {
                l_reserved, 
                l3, 
                l2, 
                l1
            };

            enum ChannelEnum
            {
                stereo, 
                joint_stereo, 
                dual_channel, 
                single_channel, 
            };

        public:
            MpaConfigHelper();

            MpaConfigHelper(
                boost::uint8_t const * buf, 
                boost::uint32_t size);

            MpaConfigHelper(
                std::vector<boost::uint8_t> const & buf);

            ~MpaConfigHelper();

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
            MpaConfig const & data() const
            {
                return *data_;
            }

        public:
            VersionEnum get_version() const;

            LayerEnum get_layer() const;

            boost::uint32_t get_bitrate() const;

            boost::uint32_t get_frequency() const;

            boost::uint32_t get_channel_count() const;

            boost::uint32_t get_sample_per_frame() const;

            boost::uint32_t get_frame_size() const;

        public:
            void set_version(
                VersionEnum version);

            void set_layer(
                LayerEnum layer);

            void set_bitrate(
                boost::uint32_t bitrate);

            void set_frequency(
                boost::uint32_t frequency);

            void set_channel_count(
                boost::uint32_t channel_count);

        public:
            bool ready() const;

            void set_format(
                ppbox::avbase::AudioInfo const & info);

            void get_format(
                ppbox::avbase::AudioInfo & info) const;

        private:
            MpaConfig * data_;

        private:
            static boost::uint32_t const bitrate_free = 0;
            static boost::uint32_t const bitrate_bad = 1;
            static boost::uint32_t const bitrate_table[2][4][16];

            static boost::uint32_t const frequency_table[2][4];

            static boost::uint32_t const channel_count_table[4];

            static boost::uint32_t const sample_per_frame_table[2][4];
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_MPA_MPA_CONFIG_HELPER_H_
