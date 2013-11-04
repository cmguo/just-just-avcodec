// FFMpegAudioDecoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/ffmpeg/FFMpegCodecMap.h"

#include <util/buffers/BuffersCopy.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

namespace ppbox
{
    namespace avcodec
    {

        struct FFMpegAudioDecoderImpl
        {
            AVCodecContext * ctx;
            AVFrame * frame;
            int got_frame;
            SwrContext *swr_ctx;
            struct AudioParams {
                int sample_rate;
                int channels;
                int64_t channel_layout;
                enum AVSampleFormat sample_fmt;
            } psrc, pdst;
            uint8_t * buf;
            unsigned int buf_size;
            uint8_t * data;
            unsigned int data_size;

            FFMpegAudioDecoderImpl()
                : ctx(NULL)
                , frame(NULL)
                , got_frame(0)
                , swr_ctx(NULL)
                , buf(NULL)
                , buf_size(0)
                , data(NULL)
                , data_size(0)
            {
                avcodec_register_all();

                ctx = avcodec_alloc_context3(NULL);
                frame = avcodec_alloc_frame();
                ctx->thread_count          = 1;
                ctx->thread_type           = 0;
                ctx->err_recognition       = AV_EF_CAREFUL;
            }

            ~FFMpegAudioDecoderImpl()
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
                    StreamType::AUDI, input_format.sub_type);
                if (codec_type == NULL) {
                    return false;
                }
                ctx->sample_rate           = input_format.audio_format.sample_rate;
                ctx->channels              = input_format.audio_format.channel_count;
                if (!input_format.format_data.empty()) {
                    ctx->extradata = (uint8_t *)av_malloc(input_format.format_data.size());
                    memcpy(ctx->extradata, &input_format.format_data.at(0), input_format.format_data.size());
                    ctx->extradata_size = input_format.format_data.size();
                }
                AVCodec * codec = avcodec_find_decoder((AVCodecID)codec_type->ffmpeg_type);
                int result = avcodec_open2(ctx, codec, NULL);
                if (result == 0) {
                    pdst.sample_rate = ctx->sample_rate;
                    pdst.channels = ctx->channels;
                }
                pdst.channel_layout = av_get_default_channel_layout(pdst.channels);
                if (output_format.sub_type == AudioSubType::PCM) {
                    pdst.sample_fmt = AV_SAMPLE_FMT_S16;
                } else if (output_format.sub_type == AudioSubType::FLT) {
                    pdst.sample_fmt = AV_SAMPLE_FMT_FLT;
                }
                output_format.audio_format.sample_size = av_get_bytes_per_sample(pdst.sample_fmt) * 8;
                psrc = pdst;
                return make_ec(result, ec);
            }

            bool push(
                Sample const & sample, 
                boost::system::error_code & ec)
            {
                AVPacket pkt;
                av_init_packet(&pkt);
                avcodec_get_frame_defaults(frame);
                for (size_t i = 0; i < sample.data.size(); ++i) {
                    pkt.data = (uint8_t *)boost::asio::buffer_cast<uint8_t const *>(sample.data[i]);
                    pkt.size = boost::asio::buffer_size(sample.data[i]);
                    while (pkt.size) {
                        int used_bytes = avcodec_decode_audio4(ctx, frame, &got_frame, &pkt);
                        if (used_bytes < 0) {
                            return make_ec(used_bytes, ec);
                        } else if(used_bytes == 0 && !got_frame) {
                            break;
                        }
                        pkt.data += used_bytes;
                        pkt.size -= used_bytes;
                    }
                }
                assert(got_frame && pkt.size == 0);
                return got_frame > 0;
            }

            bool push(
                Transcoder::eos_t const & eos, 
                boost::system::error_code & ec)
            {
                avcodec_flush_buffers(ctx);
            }

            bool pop(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                int result = convert();
                got_frame = 0;
                if (result == 0) {
                    sample.size = data_size;
                    sample.data.clear();
                    sample.data.push_back(boost::asio::buffer(data, data_size));
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

                data = frame->data[0];
                data_size = av_samples_get_buffer_size(
                    NULL, 
                    av_frame_get_channels(frame),
                    frame->nb_samples,
                    (AVSampleFormat)frame->format, 
                    1);

                AVSampleFormat sample_fmt = (AVSampleFormat)frame->format;
                int64_t channel_layout =
                    (frame->channel_layout 
                    && av_frame_get_channels(frame) == av_get_channel_layout_nb_channels(frame->channel_layout)) 
                    ? frame->channel_layout 
                    : av_get_default_channel_layout(av_frame_get_channels(frame));

                if (sample_fmt != psrc.sample_fmt 
                    || channel_layout != psrc.channel_layout 
                    || frame->sample_rate != psrc.sample_rate 
                    || !swr_ctx) {
                        swr_free(&swr_ctx);
                        swr_ctx = swr_alloc_set_opts(
                            NULL,
                            pdst.channel_layout, 
                            pdst.sample_fmt, 
                            pdst.sample_rate,
                            channel_layout, 
                            sample_fmt, 
                            frame->sample_rate,
                            0, 
                            NULL);
                        if (!swr_ctx || (result = swr_init(swr_ctx)) < 0) {
                            fprintf(stderr, "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                                frame->sample_rate, av_get_sample_fmt_name(sample_fmt), av_frame_get_channels(frame),
                                pdst.sample_rate, av_get_sample_fmt_name(pdst.sample_fmt), pdst.channels);
                            return result;
                        }
                        psrc.channel_layout = channel_layout;
                        psrc.channels = av_frame_get_channels(frame);
                        psrc.sample_rate = frame->sample_rate;
                        psrc.sample_fmt = sample_fmt;
                }

                if (swr_ctx) {
                    const uint8_t **in = (const uint8_t **)frame->extended_data;
                    uint8_t **out = &buf;
                    int out_count = (int64_t)frame->nb_samples * pdst.sample_rate / frame->sample_rate + 256;
                    int out_size  = av_samples_get_buffer_size(NULL, pdst.channels, out_count, pdst.sample_fmt, 0);
                    av_fast_malloc(&buf, &buf_size, out_size);
                    if (!buf)
                        return AVERROR(ENOMEM);
                    int len2 = swr_convert(swr_ctx, out, out_count, in, frame->nb_samples);
                    if (len2 < 0) {
                        fprintf(stderr, "swr_convert() failed\n");
                        return len2;
                    }
                    if (len2 == out_count) {
                        fprintf(stderr, "warning: audio buffer is probably too small\n");
                        swr_init(swr_ctx);
                    }
                    data = buf;
                    data_size = len2 * pdst.channels * av_get_bytes_per_sample(pdst.sample_fmt);
                }
                return 0;
            }
        };

    } // namespace avcodec
} // namespace ppbox
