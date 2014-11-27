// FFMpegAudioDecoderImpl.h

#include "just/avcodec/AudioType.h"
#include "just/avcodec/ffmpeg/FFMpegDecoderImpl.h"

extern "C"
{
#include <libswresample/swresample.h>
}

namespace just
{
    namespace avcodec
    {

        struct FFMpegAudioDecoderImpl
            : FFMpegDecoderImpl
        {
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
                : FFMpegDecoderImpl(avcodec_decode_audio4)
                , swr_ctx(NULL)
                , buf(NULL)
                , buf_size(0)
                , data(NULL)
                , data_size(0)
            {
            }

            ~FFMpegAudioDecoderImpl()
            {
            }

            bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                ctx->sample_rate = input_format.audio_format.sample_rate;
                ctx->channels = input_format.audio_format.channel_count;
                ctx->block_align = input_format.audio_format.block_align;
                bool result = FFMpegDecoderImpl::open(input_format, output_format, ec);
                if (!result) {
                    return false;
                }
                pdst.sample_rate = ctx->sample_rate;
                pdst.channels = ctx->channels;
                pdst.channel_layout = av_get_default_channel_layout(pdst.channels);
                if (output_format.sub_type == AudioType::PCM) {
                    pdst.sample_fmt = AV_SAMPLE_FMT_S16;
                } else if (output_format.sub_type == AudioType::FLT) {
                    pdst.sample_fmt = AV_SAMPLE_FMT_FLT;
                }
                int bytes_per_sample = av_get_bytes_per_sample(pdst.sample_fmt);
                output_format.audio_format.channel_count = pdst.channels;
                output_format.audio_format.sample_rate = pdst.sample_rate;
                output_format.audio_format.sample_size = bytes_per_sample * 8;
                output_format.audio_format.block_align = bytes_per_sample * pdst.channels;
                output_format.bitrate = bytes_per_sample * 8 * pdst.channels * pdst.sample_rate;
                psrc = pdst;
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
                    sample.size = data_size;
                    sample.data.clear();
                    sample.data.push_back(boost::asio::buffer(data, data_size));
                }
                return make_ec(result, ec);
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
} // namespace just
