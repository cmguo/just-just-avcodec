// FFMpegVideoDecoder.h

#ifndef _JUST_AACODEC_FFMPEG_FFMPEG_VIDEO_DECODER_H_
#define _JUST_AACODEC_FFMPEG_FFMPEG_VIDEO_DECODER_H_

#include "just/avcodec/Transcoder2.h"

struct AVCodecContext;

namespace just
{
    namespace avcodec
    {

        struct FFMpegVideoDecoderImpl;

        class FFMpegVideoDecoder
            : public Transcoder2
        {
        public:
            FFMpegVideoDecoder();

            virtual ~FFMpegVideoDecoder();

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
            FFMpegVideoDecoderImpl * impl_;
        };

        JUST_REGISTER_TRANSCODER("ffmpeg-video-dec", 10, FFMpegVideoDecoder);

    } // namespace avcodec
} // namespace just

#endif // _JUST_AACODEC_FFMPEG_FFMPEG_VIDEO_DECODER_H_
