// FFMpegAudioDecoder.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/ffmpeg/FFMpegAudioDecoder.h"
#include "ppbox/avcodec/CodecType.h"

#include <util/buffers/BuffersCopy.h>

extern "C"
{
#include <libavcodec/avcodec.h>
}

namespace ppbox
{
    namespace avcodec
    {

        struct ffmpeg_audio_codec
        {
            boost::uint32_t type;
            AVCodecID ffmpeg_type;
        };

        static ffmpeg_audio_codec const ffmpeg_audio_codec_table[] = {
            {AudioSubType::MP1A, AV_CODEC_ID_MP1},
            {AudioSubType::MP2A, AV_CODEC_ID_MP2},
            {AudioSubType::MP4A, AV_CODEC_ID_AAC},
            {AudioSubType::AC3, AV_CODEC_ID_AC3},
            {AudioSubType::FLAC, AV_CODEC_ID_FLAC},
            {AudioSubType::VORB, AV_CODEC_ID_VORBIS},
        };

        struct ffmpeg_audio_decode_context
        {
            ffmpeg_audio_decode_context()
                : ctx(NULL)
                , frame(NULL)
                , got_frame(0)
            {
                avcodec_register_all();
            }

            AVCodecContext * ctx;
            AVFrame * frame;
            int got_frame;

            int alloc()
            {
                ctx = avcodec_alloc_context3(NULL);
                frame = avcodec_alloc_frame();
                ctx->thread_count          = 1;
                ctx->thread_type           = 0;
                ctx->err_recognition       = AV_EF_CAREFUL;
                return 0;
            }

            int open(
                AVCodecID id)
            {
                AVCodec * codec = avcodec_find_decoder(id);
                return avcodec_open2(ctx, codec, NULL);
            }

            int decode(
                boost::uint8_t const * data, 
                boost::uint32_t size)
            {
                AVPacket pkt;
                av_init_packet(&pkt);
                pkt.data = (uint8_t *)data;
                pkt.size = size;
                avcodec_get_frame_defaults(frame);
                while (pkt.size) {
                    int used_bytes = avcodec_decode_audio4(ctx, frame, &got_frame, &pkt);
                    if (used_bytes < 0) {
                        return used_bytes;
                    } else if(used_bytes == 0 && !got_frame) {
                        break;
                    }
                    pkt.data += used_bytes;
                    pkt.size -= used_bytes;
                }
                return got_frame;
            }

            void flush()
            {
                avcodec_flush_buffers(ctx);
            }

            int close()
            {
                return avcodec_close(ctx);
            }

            void free()
            {
                avcodec_free_frame(&frame);
                av_freep(&ctx);
            }

            boost::uint8_t * data() const
            {
                return frame->data[0];
            }

            boost::uint32_t size() const
            {
                return frame->nb_samples * frame->channels * av_get_bytes_per_sample(ctx->request_sample_fmt);
            }
        };

        FFMpegAudioDecoder::FFMpegAudioDecoder()
            : Transcoder2(StreamType::AUDI)
            , ctx_(new ffmpeg_audio_decode_context())
        {
            for (size_t i = 0; i < sizeof(ffmpeg_audio_codec_table) / sizeof(ffmpeg_audio_codec_table[0]); ++i) {
                register_codec(ffmpeg_audio_codec_table[i].type, AudioSubType::PCM);
                register_codec(ffmpeg_audio_codec_table[i].type, AudioSubType::FLT);
            }
        }

        FFMpegAudioDecoder::~FFMpegAudioDecoder()
        {
            delete ctx_;
        }

        bool FFMpegAudioDecoder::config(
            std::map<std::string, std::string> const & config, 
            boost::system::error_code & ec)
        {
            return true;
        }

        struct ffmpeg_audio_codec_equal_type
        {
            ffmpeg_audio_codec_equal_type(
                boost::uint32_t type)
                : type_(type)
            {
            }

            bool operator()(
                ffmpeg_audio_codec const & l) const
            {
                return l.type == type_;
            }

        private:
            boost::uint32_t type_;
        };

        bool FFMpegAudioDecoder::open(
            StreamInfo const & input_format, 
            StreamInfo & output_format, 
            boost::system::error_code & ec)
        {
            ffmpeg_audio_codec const * codec_type = std::find_if(
                ffmpeg_audio_codec_table, 
                ffmpeg_audio_codec_table + sizeof(ffmpeg_audio_codec_table) / sizeof(ffmpeg_audio_codec_table[0]), 
                ffmpeg_audio_codec_equal_type(input_format.sub_type));
            if (codec_type == ffmpeg_audio_codec_table + sizeof(ffmpeg_audio_codec_table) / sizeof(ffmpeg_audio_codec_table[0])) {
                return false;
            }
            ctx_->alloc();
            AVCodecContext * ctx = ctx_->ctx;
            ctx->sample_rate           = input_format.audio_format.sample_rate;
            ctx->channels              = input_format.audio_format.channel_count;
            //ctx->bit_rate              = input_format.audio_format.sample_rate << 3;
            //ctx->bits_per_coded_sample = input_format.audio_format.sample_size;
            if (output_format.sub_type == AudioSubType::PCM) {
            //    ctx->request_sample_fmt = AV_SAMPLE_FMT_S16;
            //    ctx->bits_per_coded_sample = 16;
                output_format.audio_format.sample_size = 16;
            } else if (output_format.sub_type == AudioSubType::FLT) {
            //    ctx->request_sample_fmt = AV_SAMPLE_FMT_FLTP;
            //    ctx->bits_per_coded_sample = 32;
                output_format.audio_format.sample_size = 32;
            }
            ctx->block_align           = 1;
            ctx->err_recognition       = AV_EF_CAREFUL;
            if (!input_format.format_data.empty()) {
                ctx->extradata = (uint8_t *)av_malloc(input_format.format_data.size());
                memcpy(ctx->extradata, &input_format.format_data.at(0), input_format.format_data.size());
                ctx->extradata_size = input_format.format_data.size();
            }
            ctx_->open(codec_type->ffmpeg_type);
            return true;
        }

        static boost::uint8_t const * copy_sample_data(
            Sample const & sample)
        {
            static std::vector<boost::uint8_t> sample_buffer;
            if (sample.data.size() == 1) {
                return boost::asio::buffer_cast<boost::uint8_t const *>(sample.data[0]);
            } else {
                if (sample_buffer.size() < sample.size)
                    sample_buffer.resize(sample.size);
                util::buffers::buffers_copy(boost::asio::buffer(sample_buffer), sample.data);
                return sample_buffer.size() ? &sample_buffer.at(0) : NULL;
            }
        }

        bool FFMpegAudioDecoder::push(
            Sample const & sample, 
            boost::system::error_code & ec)
        {
            int ret = ctx_->decode(copy_sample_data(sample), sample.size);
            return true;
        }

        bool FFMpegAudioDecoder::push(
            eos_t const & eos, 
            boost::system::error_code & ec)
        {
            return true;
        }

        bool FFMpegAudioDecoder::pop(
            Sample & sample, 
            boost::system::error_code & ec)
        {
            if (ctx_->got_frame) {
                sample.data.clear();
                sample.data.push_back(boost::asio::buffer(ctx_->data(), ctx_->size())); 
            }
            return true;
        }

        bool FFMpegAudioDecoder::refresh(
            boost::system::error_code & ec)
        {
            ctx_->flush();
            return true;
        }

        bool FFMpegAudioDecoder::close(
            boost::system::error_code & ec)
        {
            ctx_->close();
            return true;
        }

    } // namespace avcodec
} // namespace ppbox
