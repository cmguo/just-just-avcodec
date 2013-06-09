// ColorSpace.h

#ifndef _PPBOX_AVCODEC_CSP_COLOR_SPACE_H_
#define _PPBOX_AVCODEC_CSP_COLOR_SPACE_H_

#include "ppbox/avcodec/StreamInfo.h"
#include "ppbox/avcodec/Sample.h"

namespace ppbox
{
    namespace avcodec
    {

        class ColorSpace
        {
        public:
            struct PictureSize
            {
                PictureSize()
                    : nplane(0)
                    , total_size(0)
                {
                    stride_sizes[0] = stride_sizes[1] = stride_sizes[2] = 0;
                    plane_sizes[0] = plane_sizes[1] = plane_sizes[2] = 0;
                }

                boost::uint32_t nplane;
                boost::uint32_t total_size;
                boost::uint32_t stride_sizes[3];
                boost::uint32_t plane_sizes[3];
            };

            static bool picture_size(
                StreamInfo const & info, 
                PictureSize & size, 
                boost::system::error_code & ec);

            typedef void (*transfer_t)(
                PictureSize const & pic, 
                boost::uint8_t * data);

            static void transfer_nv21_nv12(
                PictureSize const & pic, 
                boost::uint8_t * data);
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_CSP_COLOR_SPACE_H_