// FFMpegVideoDecoderImpl.h

#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/ffmpeg/FFMpegCodecMap.h"

#include <ppbox/avbase/TypeMap.h>

#include <util/buffers/BuffersCopy.h>

extern "C"
{
#define UINT64_C(c)   c ## ULL
#include <libavcodec/avcodec.h>
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
        {
            AVCodecContext * ctx;
            AVFrame * frame;
            int got_frame;
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
                : ctx(NULL)
                , frame(NULL)
                , got_frame(0)
                , sws_ctx(NULL)
                , sws_flags_(SWS_BICUBIC)
                , pic_size(0)
            {
                avcodec_register_all();

                ctx = avcodec_alloc_context3(NULL);
                frame = avcodec_alloc_frame();
                ctx->thread_count          = 1;
                ctx->thread_type           = 0;
                ctx->err_recognition       = AV_EF_CAREFUL;

                memset(&pic, 0, sizeof(pic));
            }

            ~FFMpegVideoDecoderImpl()
            {
                avcodec_free_frame(&frame);
                av_freep(&ctx);
            }

            bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec)
            {
                return true;
            }

            bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                FFMpegCodec const * codec_type = FFMpegCodecMap::find_by_type(
                    StreamType::VIDE, input_format.sub_type);
                if (codec_type == NULL) {
                    return false;
                }
                ctx->width = input_format.video_format.width;
                ctx->height = input_format.video_format.height;
                ctx->time_base.den = input_format.video_format.frame_rate_num;
                ctx->time_base.num = input_format.video_format.frame_rate_den;
                if (!input_format.format_data.empty()) {
                    ctx->extradata = (uint8_t *)av_malloc(input_format.format_data.size());
                    memcpy(ctx->extradata, &input_format.format_data.at(0), input_format.format_data.size());
                    ctx->extradata_size = input_format.format_data.size();
                }
                AVCodec * codec = avcodec_find_decoder((AVCodecID)codec_type->ffmpeg_type);
                int result = avcodec_open2(ctx, codec, NULL);
                if (result == 0) {
                    pdst.width = ctx->width;
                    pdst.height = ctx->height;
                }
                ff_pixel_format const * fmt = ppbox::avbase::type_map_find(
                    ff_pixel_format_table, 
                    &ff_pixel_format::format, output_format.sub_type);
                pdst.pixel_fmt = fmt->ff_format;
                psrc = pdst;
                avpicture_alloc(&pic, pdst.pixel_fmt, pdst.width, pdst.height);
                pic_size = av_image_get_buffer_size(pdst.pixel_fmt, pdst.width, pdst.height, 1);
                return make_ec(result, ec);
            }

            bool push(
                Sample const & sample, 
                boost::system::error_code & ec)
            {
                AVPacket pkt;
                av_init_packet(&pkt);
                pkt.pts = sample.dts + sample.cts_delta;
                pkt.dts = sample.dts;
                pkt.duration = sample.duration;
                avcodec_get_frame_defaults(frame);
                for (size_t i = 0; i < sample.data.size(); ++i) {
                    pkt.data = (uint8_t *)boost::asio::buffer_cast<uint8_t const *>(sample.data[i]);
                    pkt.size = boost::asio::buffer_size(sample.data[i]);
                    while (pkt.size) {
                        int used_bytes = avcodec_decode_video2(ctx, frame, &got_frame, &pkt);
                        if (used_bytes < 0) {
                            return make_ec(used_bytes, ec);
                        } else if(used_bytes == 0 && !got_frame) {
                            break;
                        }
                        pkt.data += used_bytes;
                        pkt.size -= used_bytes;
                    }
                }
                return true;
            }

            bool push(
                Transcoder::eos_t const & eos, 
                boost::system::error_code & ec)
            {
                AVPacket pkt;
                av_init_packet(&pkt);
                int result = avcodec_decode_video2(ctx, frame, &got_frame, &pkt);
                return got_frame > 0;
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

            bool refresh(
                boost::system::error_code & ec)
            {
                avcodec_flush_buffers(ctx);
                return true;
            }

            bool close(
                boost::system::error_code & ec)
            {
                avpicture_free(&pic);
                return make_ec(avcodec_close(ctx), ec);
            }

            bool make_ec(
                int r, 
                boost::system::error_code & ec)
            {
                if (r < 0) {
                    ec = boost::system::error_code(-r, 
                        boost::system::system_category);
                    return false;
                }
                return true;
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
                sws_scale(sws_ctx, frame->data, frame->linesize,
                          0, pdst.height, pic.data, pic.linesize);

                return 0;
            }
        };

    } // namespace avcodec
} // namespace ppbox
