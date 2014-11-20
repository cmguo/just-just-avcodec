// ColorSpace.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/csp/ColorSpace.h"
#include "ppbox/avcodec/VideoType.h"

#include <ppbox/avbase/TypeMap.h>

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
            {VideoType::I420, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256/2, 256/2 } },
            {VideoType::YV12, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256/2, 256/2 } },
            {VideoType::NV12, 2, { 256*1, 256*1 },        { 256*1, 256/2 },       },
            {VideoType::NV21, 2, { 256*1, 256*1 },        { 256*1, 256/2 },       },
            {VideoType::I422, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256*1, 256*1 } },
            {VideoType::YV16, 3, { 256*1, 256/2, 256/2 }, { 256*1, 256*1, 256*1 } },
            {VideoType::NV16, 2, { 256*1, 256*1 },        { 256*1, 256*1 },       },
            {VideoType::I444, 3, { 256*1, 256*1, 256*1 }, { 256*1, 256*1, 256*1 } },
            {VideoType::YV24, 3, { 256*1, 256*1, 256*1 }, { 256*1, 256*1, 256*1 } },
            {VideoType::BGR8, 1, { 256*3 },               { 256*1 },              },
            {VideoType::BGRA, 1, { 256*4 },               { 256*1 },              },
            {VideoType::RGB8, 1, { 256*3 },               { 256*1 },              },
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
            csp_t const * csp = ppbox::avbase::type_map_find(
                csp_tab, 
                &csp_t::type, info.sub_type);
            if (csp == NULL) {
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

        inline boost::uint32_t rotate(
            boost::uint32_t v)
        {
            // 1 2 3 4
            // 2 3 4 0
            // 0 1 2 3
            return ((v << 8) & 0xff00ff00) 
                | ((v >> 8) & 0x00ff00ff);
        }

        void ColorSpace::transfer_nv21_nv12(
            PictureSize const & pic, 
            boost::uint8_t * data)
        {
            boost::uint32_t * p = 
                (boost::uint32_t *)(data + pic.plane_sizes[0]);
            boost::uint32_t * e = p + pic.plane_sizes[1] / sizeof(boost::uint32_t);
            for (; p < e; ++p) {
                *p = rotate(*p);
            }
        }

    } // namespace avcodec
} // namespace ppbox
