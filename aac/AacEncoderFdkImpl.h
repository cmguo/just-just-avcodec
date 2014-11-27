// AacEncoderFdkImpl.h

#include "just/avcodec/aac/AacConfigHelper.h"

#include <util/buffers/BuffersCopy.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/logger/VargRecord.h>
#include <framework/library/Library.h>

#include <stdint.h>

#define INT FDK_INT
#define UINT FDK_UINT
#include <fdk-aac/aacenc_lib.h>

namespace fdk_aac {

    typedef AACENC_ERROR errors;;

    class fdk_aac_category
        : public boost::system::error_category
    {
    public:
        const char* name() const
        {
            return "fdk-aac";
        }

        std::string message(int value) const
        {
            switch (value) 
            {
            case AACENC_INVALID_HANDLE:
                return "Handle passed to function call was invalid.";
            case AACENC_MEMORY_ERROR:
                return "Memory allocation failed.";
            case AACENC_UNSUPPORTED_PARAMETER:
                return "Parameter not available.";
            case AACENC_INVALID_CONFIG:
                return "Configuration not provided.";
            case AACENC_INIT_ERROR:
                return "General initialization error.";
            case AACENC_INIT_AAC_ERROR:
                return "AAC library initialization error.";
            case AACENC_INIT_SBR_ERROR:
                return "SBR library initialization error.";
            case AACENC_INIT_TP_ERROR:
                return "Transport library initialization error.";
            case AACENC_INIT_META_ERROR:
                return "Meta data library initialization error.";
            case AACENC_ENCODE_ERROR:
                return "The encoding process was interrupted by an unexpected error.";
            case AACENC_ENCODE_EOF:
                return "End of file reached.";
            default:
                return "fdk-aac: other error";
            }
        }
    };

    inline const boost::system::error_category & get_category()
    {
        static fdk_aac::fdk_aac_category instance;
        return instance;
    }


} // namespace fdk_aac

inline boost::system::error_code make_error_code(
    AACENC_ERROR e)
{
    return boost::system::error_code(
        static_cast<int>(e), fdk_aac::get_category());
}

namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<fdk_aac::errors>
        {
            BOOST_STATIC_CONSTANT(bool, value = true);
        };

    }
}

namespace just
{
    namespace avcodec
    {

        struct fdk_aac_api
        {
        protected:
            AACENC_ERROR (*aacEncOpen)(HANDLE_AACENCODER *phAacEncoder, const UINT encModules, const UINT maxChannels);
            AACENC_ERROR (*aacEncClose)(HANDLE_AACENCODER *phAacEncoder);
            AACENC_ERROR (*aacEncEncode)(const HANDLE_AACENCODER hAacEncoder, const AACENC_BufDesc *inBufDesc, const AACENC_BufDesc *outBufDesc, const AACENC_InArgs *inargs, AACENC_OutArgs *outargs);
            AACENC_ERROR (*aacEncInfo)(const HANDLE_AACENCODER hAacEncoder, AACENC_InfoStruct *pInfo);
            AACENC_ERROR (*aacEncoder_SetParam)(const HANDLE_AACENCODER hAacEncoder, const AACENC_PARAM param, const UINT value );
            UINT (*aacEncoder_GetParam)(const HANDLE_AACENCODER hAacEncoder, const AACENC_PARAM param);
            AACENC_ERROR (*aacEncGetLibInfo)(LIB_INFO *info);

            fdk_aac_api()
                : aacEncOpen(NULL)
                , aacEncClose(NULL)
                , aacEncEncode(NULL)
                , aacEncInfo(NULL)
                , aacEncoder_SetParam(NULL)
                , aacEncoder_GetParam(NULL)
                , aacEncGetLibInfo(NULL)
            {
                if (lib_.open("fdk-aac")) {
                    return;
                }
#define GET_FUNC(x) get_func(BOOST_PP_STRINGIZE(x), x)
                GET_FUNC(aacEncOpen);
                GET_FUNC(aacEncClose);
                GET_FUNC(aacEncEncode);
                GET_FUNC(aacEncInfo);
                GET_FUNC(aacEncoder_SetParam);
                GET_FUNC(aacEncoder_GetParam);
                GET_FUNC(aacEncGetLibInfo);
            }

            bool ok() const
            {
                return aacEncOpen
                    && aacEncClose
                    && aacEncEncode
                    && aacEncInfo
                    && aacEncoder_SetParam
                    && aacEncoder_GetParam
                    && aacEncGetLibInfo
                    ;
            }

        private:
            template <typename T>
            void get_func(
                char const * name,
                T *& t)
            {
                t = (T *)lib_.symbol(name);
            }

        private:
            framework::library::Library lib_;
        };

        static size_t const AAC_MAX_INPUT_BUF_NUM = 16;
        static size_t const AAC_MAX_OUTPUT_BUF_SIZE = 8192;

        struct AacEncInputBufDesc
            : AACENC_BufDesc
        {
            AacEncInputBufDesc()
            {
                numBufs = 0;
                bufs = vbufs;
                bufferIdentifiers = vbufferIdentifiers;
                bufSizes = vbufSizes;
                bufElSizes = vbufElSizes;

                for (size_t i = 0; i < AAC_MAX_INPUT_BUF_NUM; ++i) {
                    vbufferIdentifiers[i] = IN_AUDIO_DATA;
                    vbufElSizes[i] = sizeof(INT_PCM);
                }
            }

            void el_size(
                INT s) 
            {
                for (size_t i = 0; i < AAC_MAX_INPUT_BUF_NUM; ++i) {
                    vbufElSizes[i] = s;
                }
            }

            void * vbufs[AAC_MAX_INPUT_BUF_NUM];
            INT vbufferIdentifiers[AAC_MAX_INPUT_BUF_NUM];
            INT vbufSizes[AAC_MAX_INPUT_BUF_NUM];
            INT vbufElSizes[AAC_MAX_INPUT_BUF_NUM];
        };

        struct AacEncOutputBufDesc
            : AACENC_BufDesc
        {
            AacEncOutputBufDesc()
            {
                numBufs = 1;
                bufs = &buf;
                bufferIdentifiers = &bufferIdentifier;
                bufSizes = &bufSize;
                bufElSizes = &bufElSize;

                buf = new boost::uint8_t[AAC_MAX_OUTPUT_BUF_SIZE];
                bufferIdentifier = OUT_BITSTREAM_DATA;
                bufSize = AAC_MAX_OUTPUT_BUF_SIZE;
                bufElSize = sizeof(boost::uint8_t);
            }

            ~AacEncOutputBufDesc()
            {
                delete [] (boost::uint8_t *)buf;
            }

            void * buf;
            INT bufferIdentifier;
            INT bufSize;
            INT bufElSize;
        };

        struct fdk_aac_impl
            : private fdk_aac_api
        {
            fdk_aac_impl()
                : hAacEncoder_(NULL)
            {
                memset(&arg_in_, 0, sizeof(arg_in_));
                memset(&arg_out_, 0, sizeof(arg_out_));
            }

            ~fdk_aac_impl()
            {
            }

            FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("fdk_aac_impl", framework::logger::Debug);

            bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec)
            {
                if (!ok()) {
                    LOG_ERROR("[config] api not ok");
                    return false;
                }

                std::map<std::string, std::string>::const_iterator iter = config.begin();
                for (; iter != config.end(); ++iter) {
                    LOG_INFO("[config]" <<  iter->first << ": " << iter->second);
                }

                return true;
            }

            bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                assert(output_format.sub_type == AudioType::AAC);
                if (output_format.format_type != AacFormatType::adts) {
                    output_format.format_type = AacFormatType::raw;
                }

                buf_in_.el_size(input_format.audio_format.sample_size / 8);

                ec = aacEncOpen(&hAacEncoder_, 1, input_format.audio_format.channel_count);
                aacEncoder_SetParam(hAacEncoder_, AACENC_AOT, AOT_AAC_LC);
                aacEncoder_SetParam(hAacEncoder_, AACENC_SAMPLERATE, input_format.audio_format.sample_rate);
                aacEncoder_SetParam(hAacEncoder_, AACENC_CHANNELMODE, input_format.audio_format.channel_count);
                aacEncoder_SetParam(hAacEncoder_, AACENC_TRANSMUX, output_format.format_type == AacFormatType::raw ? TT_MP4_RAW : TT_MP4_ADTS);

                if (!refresh(ec)) {
                    return false;
                }

                output_format.bitrate = 0;
                output_format.audio_format = input_format.audio_format;
                // get AudioSpecificConfig
                {
                    AACENC_InfoStruct info;
                    aacEncInfo(hAacEncoder_, &info);
                    output_format.format_data.assign(info.confBuf, info.confBuf + info.confSize);

                    arg_in_.numInSamples = info.inputChannels * info.frameLength;
                }

                return true;
            }

            bool push(
                Sample const & sample, 
                boost::system::error_code & ec)
            {
                buf_in_.numBufs = sample.data.size();
                for (size_t i = 0; i < sample.data.size(); ++i) {
                    buf_in_.bufs[i] = (void *)boost::asio::buffer_cast<void const *>(sample.data[i]);
                    buf_in_.bufSizes[i] = buf_in_.bufElSizes[i] = boost::asio::buffer_size(sample.data[i]);
                }
                ec = aacEncEncode(hAacEncoder_, &buf_in_, &buf_out_, &arg_in_, &arg_out_);

                return !ec;
            }

            virtual bool push(
                Transcoder::eos_t const & eos, 
                boost::system::error_code & ec)
            {
                ec = boost::asio::error::eof;
                return false;
            }

            bool pop(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                if (arg_out_.numOutBytes) {
                    //sample.time = (intptr_t)pic_out.opaque;
                    //sample.dts = pic_out.i_dts;
                    //sample.cts_delta = boost::uint32_t(pic_out.i_pts - pic_out.i_dts);
                    sample.size = arg_out_.numOutBytes;
                    sample.data.clear();
                    sample.data.push_back(boost::asio::buffer(buf_out_.buf, arg_out_.numOutBytes));
                    arg_out_.numOutBytes = 0;
                    return true;
                } else {
                    ec = boost::asio::error::would_block;
                    return false;
                }
            }

            bool refresh(
                boost::system::error_code & ec)
            {
                ec = aacEncEncode(hAacEncoder_, NULL, NULL, NULL, NULL);
                return !ec;
            }

            bool close(
                boost::system::error_code & ec)
            {
                ec = aacEncClose(&hAacEncoder_);

                return !ec;
            }

        private:
            HANDLE_AACENCODER hAacEncoder_;
            AacEncInputBufDesc buf_in_;
            AacEncOutputBufDesc buf_out_;
            AACENC_InArgs arg_in_;
            AACENC_OutArgs arg_out_;
        };

    } // namespace avcodec
} // namespace just
