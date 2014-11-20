// FFMpegVideoDecoderImpl.h

#include "ppbox/avcodec/VideoType.h"
#include "ppbox/avcodec/ffmpeg/FFMpegDecoderImpl.h"

extern "C"
{
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

namespace ppbox
{
    namespace avcodec
    {

        struct ff_pixel_format
        {
            boost::uint32_t format;
            AVPixelFormat ff_format;
        } const ff_pixel_format_table[] = {
            {VideoSubType::I420, AV_PIX_FMT_YUV420P}, 
        };

        struct FFMpegVideoDecoderImpl
            : FFMpegDecoderImpl
        {
            SwsContext *sws_ctx;
            struct VideoParams {
                int width;
                int height;
                AVPixelFormat pixel_fmt;
            } psrc, pdst;
            int64_t sws_flags_;
            AVPicture pic;
            int pic_size;

            FFMpegVideoDecoderImpl()
                : FFMpegDecoderImpl(avcodec_decode_video2)
                , sws_ctx(NULL)
                , sws_flags_(SWS_BICUBIC)
                , pic_size(0)
            {
                memset(&pic, 0, sizeof(pic));
            }

            ~FFMpegVideoDecoderImpl()
            {
            }

            bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                ctx->width = input_format.video_format.width;
                ctx->height = input_format.video_format.height;
                ctx->time_base.den = input_format.video_format.frame_rate_num;
                ctx->time_base.num = input_format.video_format.frame_rate_den;
                bool result = FFMpegDecoderImpl::open(input_format, output_format, ec);
                if (!result) {
                    return false;
                }
                pdst.width = ctx->width;
                pdst.height = ctx->height;
                ff_pixel_format const * fmt = ppbox::avbase::type_map_find(
                    ff_pixel_format_table, 
                    &ff_pixel_format::format, output_format.sub_type);
                pdst.pixel_fmt = fmt->ff_format;
                psrc = pdst;
                output_format.video_format.width = pdst.width;
                output_format.video_format.height = pdst.height;
                output_format.video_format.frame_rate_num = ctx->time_base.den;
                output_format.video_format.frame_rate_den = ctx->time_base.num;
                avpicture_alloc(&pic, pdst.pixel_fmt, pdst.width, pdst.height);
                pic_size = av_image_get_buffer_size(pdst.pixel_fmt, pdst.width, pdst.height, 1);
                return true;
            }

            bool pop(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                if (got_frame == 0) {
                    ec = boost::asio::error::would_block;
                    return false;
                }
                int result = convert();
                got_frame = 0;
                if (result == 0) {
                    sample.dts = frame->pkt_dts;
                    sample.cts_delta = frame->pkt_pts - frame->pkt_dts;
                    sample.duration = frame->pkt_duration;
                    sample.size = pic_size;
                    sample.data.clear();
                    sample.data.push_back(boost::asio::buffer(pic.data[0], pic_size));
                }
                return make_ec(result, ec);
            }

            bool close(
                boost::system::error_code & ec)
            {
                avpicture_free(&pic);
                return FFMpegDecoderImpl::close(ec);
            }

            int convert()
            {
                int result = 0;
                
                //av_opt_get_int(sws_opts, "sws_flags", 0, &sws_flags);
                sws_ctx = sws_getCachedContext(sws_ctx,
                    frame->width, frame->height, (AVPixelFormat)frame->format, pdst.width, pdst.height,
                    pdst.pixel_fmt, sws_flags_, NULL, NULL, NULL);
                if (sws_ctx == NULL) {
                    fprintf(stderr, "Cannot initialize the conversion context\n");
                    exit(1);
                }
                result = sws_scale(sws_ctx, frame->data, frame->linesize,
                          0, pdst.height, pic.data, pic.linesize);

                return result;
            }
        };

    } // namespace avcodec
} // namespace ppbox
