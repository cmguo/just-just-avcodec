// AvcEncoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcEncoder.h"

#include <util/buffers/BuffersCopy.h>

#include <stdint.h>
#include <thirdparty/x264/x264.h>

namespace ppbox
{
    namespace avcodec
    {

        struct x264_impl
        {
            x264_impl()
                : x264(NULL)
                , panel_buffer(NULL)
                , panel_size(0)
                , p_nal(NULL)
                , i_nal(0)
            {
            }

            ~x264_impl()
            {
            }

            bool open(
                StreamInfo const & input_format, 
                std::map<std::string, std::string> const & config, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                x264_picture_alloc(&pic_in, input_format.format_type, input_format.video_format.width, input_format.video_format.height);
                panel_buffer = pic_in.img.plane[0];
                for (int i = 0; i < pic_in.img.i_plane; ++i) {
                    panel_size += pic_in.img.i_stride[i];
                }

                x264_param_default(&param);
                std::map<std::string, std::string>::const_iterator iter = config.begin();
                for (; iter != config.end(); ++iter) {
                    x264_param_parse(&param, iter->first.c_str(), iter->second.c_str());
                }
                x264 = x264_encoder_open(&param);
                x264_encoder_parameters(x264, &param);
                // get sps pps
                {
                    x264_encoder_headers(x264, &p_nal, &i_nal);
                    for (int i = 0; i < i_nal; ++i) {
                        output_format.format_data.insert(output_format.format_data.end(), 
                            p_nal[i].p_payload, p_nal[i].p_payload + p_nal[i].i_payload);
                    }
                }

                return true;
            }

            bool push(
                Sample const & sample, 
                boost::system::error_code & ec)
            {
                assert(sample.size == panel_size);

                pic_in.i_pts = sample.dts;
                if (sample.data.size() == 1) {
                    boost::uint8_t * data = const_cast<uint8_t *>(boost::asio::buffer_cast<boost::uint8_t const *>(sample.data.front()));
                    boost::uint8_t * end = data + boost::asio::buffer_size(sample.data.front());
                    for (int i = 0; i < pic_in.img.i_plane; ++i) {
                        pic_in.img.plane[i] = data;
                        data += pic_in.img.i_stride[i];
                    }
                } else {
                    util::buffers::buffers_copy(
                        boost::asio::buffer(panel_buffer, panel_size), 
                        sample.data);
                }
                x264_encoder_encode(x264, &p_nal, &i_nal, &pic_in, &pic_out);

                return true;
            }

            bool pop(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                if (p_nal) {
                    if (pic_out.b_keyframe)
                        sample.flags |= sample.f_sync;
                    sample.dts = pic_out.i_dts;
                    sample.cts_delta = boost::uint32_t(pic_out.i_pts - pic_out.i_dts);
                    for (int i = 0; i < i_nal; ++i) {
                        sample.data.push_back(boost::asio::buffer(p_nal[i].p_payload, p_nal[i].i_payload));
                    }
                    p_nal = NULL;
                    ec.clear();
                    return true;
                } else {
                    ec = boost::asio::error::would_block;
                    return false;
                }
            }

            bool close(
                boost::system::error_code & ec)
            {
                x264_encoder_close(x264);
                x264 = NULL;

                pic_in.img.plane[0] = panel_buffer;
                x264_picture_clean(&pic_in);

                return true;
            }

        private:
            x264_t * x264;
            x264_param_t param;
            x264_picture_t pic_in;
            uint8_t * panel_buffer;
            int panel_size;
            x264_picture_t pic_out;
            x264_nal_t * p_nal;
            int i_nal;
        };

    } // namespace avcodec
} // namespace ppbox
