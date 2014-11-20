// FFMpegCodecMap.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/ffmpeg/FFMpegCodecMap.h"
#include "ppbox/avcodec/VideoType.h"
#include "ppbox/avcodec/AudioType.h"

#include <ppbox/avbase/TypeMap.h>

extern "C"
{
#define UINT64_C(c)   c ## ULL
#include <libavcodec/avcodec.h>
}

namespace ppbox
{
    namespace avcodec
    {


        FFMpegCodec const FFMpegCodecMap::table_[] = {
            {StreamType::VIDE, VideoType::AVC,  AV_CODEC_ID_H264}, 
            {StreamType::VIDE, VideoType::MP4V, AV_CODEC_ID_MPEG4}, 
            {StreamType::VIDE, VideoType::WMV2, AV_CODEC_ID_WMV2}, 
            {StreamType::VIDE, VideoType::WMV3, AV_CODEC_ID_WMV3}, 
            {StreamType::VIDE, VideoType::MP41, AV_CODEC_ID_MSMPEG4V1}, 
            {StreamType::VIDE, VideoType::MP42, AV_CODEC_ID_MSMPEG4V2}, 
            {StreamType::VIDE, VideoType::MP43, AV_CODEC_ID_MSMPEG4V3}, 
            {StreamType::VIDE, VideoType::RV30, AV_CODEC_ID_RV30}, 
            {StreamType::VIDE, VideoType::RV40, AV_CODEC_ID_RV40}, 
            {StreamType::VIDE, VideoType::HEVC, AV_CODEC_ID_HEVC },

            {StreamType::AUDI, AudioType::MP1,  AV_CODEC_ID_MP1},
            {StreamType::AUDI, AudioType::MP2,  AV_CODEC_ID_MP2},
            {StreamType::AUDI, AudioType::MP3,  AV_CODEC_ID_MP3},
            {StreamType::AUDI, AudioType::MP1A, AV_CODEC_ID_MP3},
            {StreamType::AUDI, AudioType::MP2A, AV_CODEC_ID_MP3},
            {StreamType::AUDI, AudioType::AAC,  AV_CODEC_ID_AAC},
            {StreamType::AUDI, AudioType::WMA2, AV_CODEC_ID_WMAV2},
            {StreamType::AUDI, AudioType::AC3,  AV_CODEC_ID_AC3},
            {StreamType::AUDI, AudioType::EAC3, AV_CODEC_ID_EAC3},
            {StreamType::AUDI, AudioType::FLAC, AV_CODEC_ID_FLAC},
            {StreamType::AUDI, AudioType::VORB, AV_CODEC_ID_VORBIS},
            {StreamType::AUDI, AudioType::COOK, AV_CODEC_ID_COOK},
        };

        FFMpegCodec const * FFMpegCodecMap::find_by_type(
            boost::uint32_t category, 
            boost::uint32_t type)
        {
            FFMpegCodec const * codec = ppbox::avbase::type_map_find(
                table_, 
                &FFMpegCodec::category, category, 
                &FFMpegCodec::type, type);
            return codec;
        }

        FFMpegCodec const * FFMpegCodecMap::find_by_ffmpeg_type(
            boost::uint32_t category, 
            boost::uint32_t ffmpeg_type)
        {
            FFMpegCodec const * codec = ppbox::avbase::type_map_find(
                table_, 
                &FFMpegCodec::category, category, 
                &FFMpegCodec::ffmpeg_type, ffmpeg_type);
            return codec;
        }

        FFMpegCodec const * FFMpegCodecMap::table()
        {
            return table_;
        }

        size_t FFMpegCodecMap::count()
        {
            return sizeof(table_) / sizeof(table_[0]);
        }

    } // namespace avcodec
} // namespace ppbox
