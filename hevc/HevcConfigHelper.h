// HevcConfigHelper.h

#ifndef _JUST_HEVCODEC_HEVC_HEVC_CONFIG_HELPER_H_
#define _JUST_HEVCODEC_HEVC_HEVC_CONFIG_HELPER_H_

namespace just
{
    namespace avbase
    {
        struct VideoInfo;
    }

    namespace avcodec
    {

        struct HevcConfig;

        class HevcConfigHelper
        {
        public:
            HevcConfigHelper();

            HevcConfigHelper(
                boost::uint8_t const * buf, 
                boost::uint32_t size);

            HevcConfigHelper(
                std::vector<boost::uint8_t> const & buf);

            ~HevcConfigHelper();

        public:
            HevcConfig const & data() const
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
            typedef std::vector<std::vector<boost::uint8_t> > param_set_t;

            param_set_t const & param_set(
                boost::uint8_t type) const;

            param_set_t const & vps() const;

            param_set_t const & sps() const;

            param_set_t const & pps() const;

        public:
            bool ready() const;

            void get_format(
                just::avbase::VideoInfo & info) const;

        private:
            HevcConfig * data_;
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_HEVCODEC_HEVC_HEVC_CONFIG_HELPER_H_
