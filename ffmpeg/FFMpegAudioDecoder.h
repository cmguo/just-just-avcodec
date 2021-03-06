// FFMpegAudioDecoder.h

#ifndef _JUST_AACODEC_FFMPEG_FFMPEG_AUDIO_DECODER_H_
#define _JUST_AACODEC_FFMPEG_FFMPEG_AUDIO_DECODER_H_

#include "just/avcodec/Transcoder2.h"

struct AVCodecContext;

namespace just
{
    namespace avcodec
    {

        struct FFMpegAudioDecoderImpl;

        class FFMpegAudioDecoder
            : public Transcoder2
        {
        public:
            FFMpegAudioDecoder();

            virtual ~FFMpegAudioDecoder();

        public:
            virtual bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec);

            virtual bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec);

            virtual bool push(
                Sample const & sample, 
                boost::system::error_code & ec);

            virtual bool push(
                eos_t const & eos, 
                boost::system::error_code & ec);

            virtual bool pop(
                Sample & sample, 
                boost::system::error_code & ec);

            virtual bool refresh(
                boost::system::error_code & ec);

            virtual bool close(
                boost::system::error_code & ec);

        private:
            FFMpegAudioDecoderImpl * impl_;
        };

        JUST_REGISTER_TRANSCODER("ffmpeg-audio-dec", 10, FFMpegAudioDecoder);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AACODEC_FFMPEG_FFMPEG_AUDIO_DECODER_H_
