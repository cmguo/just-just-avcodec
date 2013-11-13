// FFMpegCodecMap.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/ffmpeg/FFMpegCodecMap.h"
#include "ppbox/avcodec/CodecType.h"

#include <ppbox/avbase/TypeMap.h>

extern "C"
{
#include <libavcodec/avcodec.h>
}

namespace ppbox
{
    namespace avcodec
    {


        FFMpegCodec const FFMpegCodecMap::table_[] = {
            {StreamType::VIDE, VideoSubType::AVC1, AV_CODEC_ID_H264}, 
            {StreamType::VIDE, VideoSubType::MP4V, AV_CODEC_ID_MPEG4}, 
            {StreamType::VIDE, VideoSubType::MP41, AV_CODEC_ID_MSMPEG4V1}, 
            {StreamType::VIDE, VideoSubType::MP42, AV_CODEC_ID_MSMPEG4V2}, 
            {StreamType::VIDE, VideoSubType::MP43, AV_CODEC_ID_MSMPEG4V3}, 

            {StreamType::AUDI, AudioSubType::MP1A, AV_CODEC_ID_MP3},
            {StreamType::AUDI, AudioSubType::MP1A, AV_CODEC_ID_MP1},
            {StreamType::AUDI, AudioSubType::MP2A, AV_CODEC_ID_MP2},
            {StreamType::AUDI, AudioSubType::MP4A, AV_CODEC_ID_AAC},
            {StreamType::AUDI, AudioSubType::AC3,  AV_CODEC_ID_AC3},
            {StreamType::AUDI, AudioSubType::AC3, AV_CODEC_ID_EAC3},
            {StreamType::AUDI, AudioSubType::FLAC, AV_CODEC_ID_FLAC},
            {StreamType::AUDI, AudioSubType::VORB, AV_CODEC_ID_VORBIS},
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
