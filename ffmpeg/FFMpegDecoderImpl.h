// FFMpegDecoderImpl.h

#include "ppbox/avcodec/CodecType.h"
#include "ppbox/avcodec/ffmpeg/FFMpegCodecMap.h"
#include "ppbox/avcodec/ffmpeg/FFMpegLog.h"

#include <ppbox/avbase/TypeMap.h>

#include <util/buffers/BuffersCopy.h>

extern "C"
{
#define UINT64_C(c)   c ## ULL
#include <libavcodec/avcodec.h>
}

namespace ppbox
{
    namespace avcodec
    {

        struct FFMpegDecoderImpl
        {
        protected:
            typedef int (*decoder_t)(
                AVCodecContext *avctx, 
                AVFrame *frame, 
                int *got_frame_ptr, 
                const AVPacket *avpkt);

            AVCodecContext * ctx;
            AVFrame * frame;
            int got_frame;
            decoder_t decoder;
            std::vector<uint8_t> sample_buffer;

            FFMpegDecoderImpl(
                decoder_t decoder)
                : ctx(NULL)
                , frame(NULL)
                , got_frame(0)
                , decoder(decoder)
            {
                ffmpeg_log_setup();
                avcodec_register_all();

                ctx = avcodec_alloc_context3(NULL);
                frame = avcodec_alloc_frame();
                ctx->thread_count          = 1;
                ctx->thread_type           = 0;
                ctx->err_recognition       = AV_EF_CAREFUL;
            }

            ~FFMpegDecoderImpl()
            {
                avcodec_free_frame(&frame);
                av_freep(&ctx);
            }

        public:
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
                    input_format.type, input_format.sub_type);
                if (codec_type == NULL) {
                    return false;
                }
                if (!input_format.format_data.empty()) {
                    ctx->extradata = (uint8_t *)av_malloc(input_format.format_data.size());
                    memcpy(ctx->extradata, &input_format.format_data.at(0), input_format.format_data.size());
                    ctx->extradata_size = input_format.format_data.size();
                }
                AVCodec * codec = avcodec_find_decoder((AVCodecID)codec_type->ffmpeg_type);
                int result = avcodec_open2(ctx, codec, NULL);
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
                pkt.data = (uint8_t *)copy_sample_data(sample);
                pkt.size = sample.size;
                while (pkt.size) {
                    int used_bytes = decoder(ctx, frame, &got_frame, &pkt);
                    if (used_bytes < 0) {
                        return make_ec(used_bytes, ec);
                    } else if(used_bytes == 0 && !got_frame) {
                        break;
                    }
                    pkt.data += used_bytes;
                    pkt.size -= used_bytes;
                }
                return true;
            }

            bool push(
                Transcoder::eos_t const & eos, 
                boost::system::error_code & ec)
            {
                AVPacket pkt;
                av_init_packet(&pkt);
                pkt.data = NULL;
                pkt.size = 0;
                int result = decoder(ctx, frame, &got_frame, &pkt);
                return got_frame > 0;
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

        protected:
            uint8_t const * copy_sample_data(
                Sample const & sample)
            {
                if (sample.data.size() == 1) {
                    return boost::asio::buffer_cast<uint8_t const *>(sample.data.front());
                } else {
                    if (sample_buffer.size() < sample.size) {
                        sample_buffer.resize(sample.size, 0);
                    }
                    util::buffers::buffers_copy(
                        boost::asio::buffer(sample_buffer), 
                        sample.data);
                    return &sample_buffer.at(0);
                }
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
        };

    } // namespace avcodec
} // namespace ppbox
