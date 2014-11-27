// FFMpegCodecMap.h

#ifndef _JUST_AVCODEC_FFMPEG_FFMPEG_CODEC_MAP_H_
#define _JUST_AVCODEC_FFMPEG_FFMPEG_CODEC_MAP_H_

namespace just
{
    namespace avcodec
    {

        struct FFMpegCodec
        {
            boost::uint32_t category;
            boost::uint32_t type;
            boost::uint32_t ffmpeg_type;
        };

        class FFMpegCodecMap
        {
        public:
            static FFMpegCodec const * find_by_type(
                boost::uint32_t category, 
                boost::uint32_t type);

            static FFMpegCodec const * find_by_ffmpeg_type(
                boost::uint32_t category, 
                boost::uint32_t ffmpeg_type);

            static FFMpegCodec const * table();

            static size_t count();

        private:
            static FFMpegCodec const table_[];
        };

    } // namespace avcodec
} // namespace just

#endif // _JUST_AVCODEC_FFMPEG_FFMPEG_CODEC_MAP_H_
