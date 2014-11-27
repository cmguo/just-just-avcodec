// Codec.cpp

#include "just/avcodec/Common.h"
#include "just/avcodec/Codec.h"
#include "just/avcodec/Error.h"

#include "just/avcodec/Assembler.h"
#include "just/avcodec/Splitter.h"
#include "just/avcodec/Debuger.h"

#include <util/tools/ClassRegister.h>
#include "just/avcodec/avc/AvcCodec.h"
#include "just/avcodec/hevc/HevcCodec.h"
#include "just/avcodec/mpa/MpaCodec.h"
#include "just/avcodec/aac/AacCodec.h"
#ifndef JUST_DISABLE_FFMPEG
#  include "just/avcodec/ffmpeg/FFMpegCodec.h"
#endif

namespace just
{
    namespace avcodec
    {

        Codec::Codec()
            : codec_type_(StreamSubType::NONE)
        {
        }

        Codec::~Codec()
        {
        }

        Assembler * Codec::create_assembler(
            boost::uint32_t format_type, 
            boost::system::error_code & ec)
        {
            return AssemblerFactory::create(codec_type_, format_type, ec);
        }

        Splitter * Codec::create_splitter(
            boost::uint32_t format_type, 
            boost::system::error_code & ec)
        {
            return SplitterFactory::create(codec_type_, format_type, ec);
        }

        Debuger * Codec::create_debuger(
            boost::system::error_code & ec)
        {
            return DebugerFactory::create(codec_type_, ec);
        }

        bool Codec::static_finish_stream_info(
            StreamInfo & info, 
            boost::system::error_code & ec)
        {
            Codec * codec = CodecFactory::create(info.sub_type, ec);
            if (codec) {
                bool b = codec->finish_stream_info(info, ec);
                delete codec;
                return b;
            }
            ec.clear();
            return true;
        }

        boost::system::error_code CodecTraits::error_not_found()
        {
            return error::codec_not_support;
        }

        Codec * CodecFactory::create(
            boost::uint32_t codec_type, 
            boost::system::error_code & ec)
        {
            Codec * codec = 
                util::tools::ClassFactory<CodecTraits>::create(codec_type, ec);
            if (codec)
                codec->codec_type_ = codec_type;
            return codec;
        }

    } // namespace avcodec
} // namespace just
