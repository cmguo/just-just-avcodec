// AvcConfigHelper.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_CONFIG_HELPER_H_
#define _PPBOX_AVCODEC_AVC_AVC_CONFIG_HELPER_H_

namespace ppbox
{
    namespace avbase
    {
        struct VideoInfo;
    }

    namespace avcodec
    {

        struct AvcConfig;

        class AvcConfigHelper
        {
        public:
            AvcConfigHelper();

            AvcConfigHelper(
                boost::uint8_t const * buf, 
                boost::uint32_t size);

            AvcConfigHelper(
                std::vector<boost::uint8_t> const & buf);

            ~AvcConfigHelper();

        public:
            AvcConfig const & data() const
            {
                return *data_;
            }

        public:
            void from_data(
                std::vector<boost::uint8_t> const & buf);

            void to_data(
                std::vector<boost::uint8_t> & buf) const;

        public:
            void from_es_data(
                std::vector<boost::uint8_t> const & buf);

            void to_es_data(
                std::vector<boost::uint8_t> & buf) const;

        public:
            bool ready() const;

            void get_format(
                ppbox::avbase::VideoInfo & info) const;

        private:
            AvcConfig * data_;
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_CONFIG_HELPER_H_
