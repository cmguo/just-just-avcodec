// ColorSpace.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/csp/ColorSpace.h"
#include "ppbox/avcodec/CodecType.h"

namespace ppbox
{
    namespace avcodec
    {

        struct csp_t
        {
            boost::uint32_t type;
            boost::uint32_t planes;
            boost::uint32_t width_fix8[3];
            boost::uint32_t height_fix8[3];
        };

        static csp_t const csp_tab[] = {
            {VideoSubType::I420, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256/2, 256/2 } },
            {VideoSubType::YV12, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256/2, 256/2 } },
            {VideoSubType::NV12, 2, { 256*1, 256*1 },        { 256*1, 256/2 },       },
            {VideoSubType::I422, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256*1, 256*1 } },
            {VideoSubType::YV16, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256*1, 256*1 } },
            //{VideoSubType::NV16, 2, { 256*1, 256*1 },        { 256*1, 256*1 },       },
            {VideoSubType::I444, 3, { 256*1, 256*1, 256*1 }, { 256*1, 256*1, 256*1 } },
            {VideoSubType::YV24, 3, { 256*1, 256*1, 256*1 }, { 256*1, 256*1, 256*1 } },
            {VideoSubType::BGR8, 1, { 256*3 },               { 256*1 },              },
            {VideoSubType::BGRA, 1, { 256*4 },               { 256*1 },              },
            {VideoSubType::RGB8, 1, { 256*3 },               { 256*1 },              },
        };

        struct csp_equal_type
        {
            csp_equal_type(
                boost::uint32_t type)
                : type_(type)
            {
            }

            bool operator()(
                csp_t const & l) const
            {
                return l.type == type_;
            }

        private:
            boost::uint32_t type_;
        };

        bool ColorSpace::picture_size(
            StreamInfo const & info, 
            PictureSize & size, 
            boost::system::error_code & ec)
        {
            assert(info.type == StreamType::VIDE);
            csp_t const * csp = std::find_if(csp_tab, csp_tab + sizeof(csp_tab) / sizeof(csp_tab[0]), 
                csp_equal_type(info.sub_type));
            if (csp == csp_tab + sizeof(csp_tab) / sizeof(csp_tab[0])) {
                return false;
            }
            size.nplane = csp->planes;
            for (boost::uint32_t i = 0; i < size.nplane; ++i) {
                boost::uint32_t stride_size = boost::uint32_t(((boost::uint64_t)info.video_format.width * csp->width_fix8[i]) >> 8);
                boost::uint32_t plane_size = boost::uint32_t(((boost::uint64_t)info.video_format.height * csp->height_fix8[i]) >> 8) * stride_size;
                size.total_size += plane_size;
                size.stride_sizes[i] = stride_size;
                size.plane_sizes[i] = plane_size;
            }
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
