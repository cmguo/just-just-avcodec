// AvcEncoderX264Impl.h

#include "ppbox/avcodec/avc/AvcConfigHelper.h"
#include "ppbox/avcodec/avc/AvcNaluHelper.h"
#include "ppbox/avcodec/avc/AvcFormatType.h"
#include "ppbox/avcodec/csp/ColorSpace.h"

#include <ppbox/avbase/TypeMap.h>

#include <util/buffers/BuffersCopy.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>
#include <framework/logger/VargRecord.h>
#include <framework/library/Library.h>

#include <stdint.h>

#ifdef WIN32
#  define X264_API_IMPORTS
#endif

extern "C"
{
#include <x264.h>
};

namespace ppbox
{
    namespace avcodec
    {

        struct x264_csp_t
        {
            boost::uint32_t type;
            boost::uint32_t x264_csp;
            ColorSpace::transfer_t transfer;
        };

        static x264_csp_t const x264_csp_tab[] = {
            {VideoSubType::I420, X264_CSP_I420, NULL},
            {VideoSubType::YV12, X264_CSP_YV12, NULL},
            {VideoSubType::NV12, X264_CSP_NV12, NULL},
            //{VideoSubType::NV21, X264_CSP_NV12, ColorSpace::transfer_nv21_nv12},
            {VideoSubType::I422, X264_CSP_I422, NULL},
            {VideoSubType::YV16, X264_CSP_YV16, NULL},
            {VideoSubType::NV16, X264_CSP_NV16, NULL},
            {VideoSubType::I444, X264_CSP_I444, NULL},
            {VideoSubType::YV24, X264_CSP_YV24, NULL},
            {VideoSubType::BGR8, X264_CSP_BGR , NULL},
            {VideoSubType::BGRA, X264_CSP_BGRA, NULL},
            {VideoSubType::RGB8, X264_CSP_RGB , NULL},
        };

        struct x264_csp_equal_type
        {
            x264_csp_equal_type(
                boost::uint32_t type)
                : type_(type)
            {
            }

            bool operator()(
                x264_csp_t const & l) const
            {
                return l.type == type_;
            }

        private:
            boost::uint32_t type_;
        };

        struct x264_api
        {
        protected:
            int (*x264_picture_alloc)(x264_picture_t *pic, int i_csp, int i_width, int i_height);
            void (*x264_picture_clean)(x264_picture_t *pic);
            void (*x264_param_default_preset)(x264_param_t *, const char *preset, const char *tune);
            int (*x264_param_apply_profile)(x264_param_t *, const char *profile);
            int (*x264_param_parse)(x264_param_t *, const char *name, const char *value);
            x264_t * (*x264_encoder_open)(x264_param_t *);
            void (*x264_encoder_parameters)(x264_t *, x264_param_t *);
            int (*x264_encoder_headers)(x264_t *, x264_nal_t **pp_nal, int *pi_nal);
            int (*x264_encoder_encode)(x264_t *, x264_nal_t **pp_nal, int *pi_nal, x264_picture_t *pic_in_, x264_picture_t *pic_out_);
            int (*x264_encoder_delayed_frames)(x264_t *);
            void (*x264_encoder_intra_refresh)(x264_t *);
            void (*x264_encoder_close)(x264_t *);

            x264_api()
                : x264_picture_alloc(NULL)
                , x264_picture_clean(NULL)
                , x264_param_default_preset(NULL)
                , x264_param_apply_profile(NULL)
                , x264_param_parse(NULL)
                , x264_encoder_open(NULL)
                , x264_encoder_parameters(NULL)
                , x264_encoder_headers(NULL)
                , x264_encoder_encode(NULL)
                , x264_encoder_delayed_frames(NULL)
                , x264_encoder_intra_refresh(NULL)
                , x264_encoder_close(NULL)
            {
                if (lib_.open("x264_")) {
                    return;
                }
#define GET_FUNC(x) get_func(BOOST_PP_STRINGIZE(x), x)
                GET_FUNC(x264_picture_alloc);
                GET_FUNC(x264_picture_clean);
                GET_FUNC(x264_param_default_preset);
                GET_FUNC(x264_param_apply_profile);
                GET_FUNC(x264_param_parse);
                GET_FUNC(x264_encoder_open);
                GET_FUNC(x264_encoder_parameters);
                GET_FUNC(x264_encoder_headers);
                GET_FUNC(x264_encoder_encode);
                GET_FUNC(x264_encoder_delayed_frames);
                GET_FUNC(x264_encoder_intra_refresh);
                GET_FUNC(x264_encoder_close);
            }

            bool ok() const
            {
                return x264_picture_alloc
                    && x264_picture_clean
                    && x264_param_default_preset
                    && x264_param_apply_profile
                    && x264_param_parse
                    && x264_encoder_open
                    && x264_encoder_parameters
                    && x264_encoder_headers
                    && x264_encoder_encode
                    && x264_encoder_delayed_frames
                    && x264_encoder_intra_refresh
                    && x264_encoder_close;
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

        struct x264_impl
            : private x264_api
        {
            x264_impl()
                : x264_(NULL)
                , panel_buffer_(NULL)
                , panel_size_(0)
                , p_nal_(NULL)
                , i_nal_(0)
            {
            }

            ~x264_impl()
            {
            }

            FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("x264_impl", framework::logger::Debug);

            static void log(void *, int level, char const * fmt, va_list args)
            {
                // X264_LOG_ERROR
                LOG_VARG(level + 2, (fmt, args));
            }

            bool config(
                std::map<std::string, std::string> const & config, 
                boost::system::error_code & ec)
            {
                if (!ok()) {
                    LOG_ERROR("[config] api not ok");
                    return false;
                }

                std::map<std::string, std::string>::const_iterator iter;
                char const * preset = NULL;
                char const * tune = NULL;
                if ((iter = config.find("preset")) != config.end()) {
                    preset = iter->second.c_str();
                }
                if ((iter = config.find("tune")) != config.end()) {
                    tune = iter->second.c_str();
                }
                x264_param_default_preset(&param_, preset, tune);
                param_.pf_log = log;
                iter = config.begin();
                for (; iter != config.end(); ++iter) {
                    LOG_INFO("[config]" <<  iter->first << ": " << iter->second);
                    x264_param_parse(&param_, iter->first.c_str(), iter->second.c_str());
                }
                if ((iter = config.find("profile")) != config.end()) {
                    x264_param_apply_profile(&param_, iter->second.c_str());
                }
                return true;
            }

            bool open(
                StreamInfo const & input_format, 
                StreamInfo & output_format, 
                boost::system::error_code & ec)
            {
                x264_csp_t const * csp = ppbox::avbase::type_map_find(
                    x264_csp_tab, 
                    &x264_csp_t::type, input_format.sub_type);
                if (csp == NULL) {
                    return false;
                }

                ColorSpace::picture_size(input_format, picture_, ec);
                transfer_ = csp->transfer;

                x264_picture_alloc(&pic_in_, csp->x264_csp, input_format.video_format.width, input_format.video_format.height);
                panel_buffer_ = pic_in_.img.plane[0];
                panel_size_ = picture_.total_size;

                assert(output_format.sub_type == VideoSubType::AVC1);
                if (output_format.format_type != AvcFormatType::byte_stream) {
                    output_format.format_type = AvcFormatType::packet;
                }

                param_.i_csp = csp->x264_csp;
                param_.i_width = input_format.video_format.width;
                param_.i_height = input_format.video_format.height;
                param_.i_fps_num = input_format.video_format.frame_rate_num;
                param_.i_fps_den = input_format.video_format.frame_rate_den;
                param_.b_annexb = (output_format.format_type == AvcFormatType::byte_stream);

                x264_ = x264_encoder_open(&param_);
                x264_encoder_parameters(x264_, &param_);

                output_format.bitrate = param_.rc.i_bitrate;
                output_format.video_format = input_format.video_format;
                // get sps pps
                {
                    x264_encoder_headers(x264_, &p_nal_, &i_nal_);
                    output_format.format_data.clear();
                    for (int i = 0; i < i_nal_; ++i) {
                        boost::uint8_t start_code[] = {0, 0, 0, 1};
                        output_format.format_data.insert(output_format.format_data.end(), 
                            start_code, start_code + sizeof(start_code));
                        output_format.format_data.insert(output_format.format_data.end(), 
                            p_nal_[i].p_payload + 4, p_nal_[i].p_payload + p_nal_[i].i_payload);
                    }
                    config_.from_es_data(output_format.format_data);
                    if (param_.b_annexb == 0) {
                        config_.to_data(output_format.format_data);
                    }
                    output_format.context = &config_;
                }

                return true;
            }

            bool push(
                Sample const & sample, 
                boost::system::error_code & ec)
            {
                assert(sample.size == (size_t)panel_size_);

                pic_in_.opaque = (void *)(intptr_t)sample.time;
                pic_in_.i_pts = sample.dts;
                boost::uint8_t * data = const_cast<uint8_t *>(boost::asio::buffer_cast<boost::uint8_t const *>(sample.data.front()));
                if (sample.data.size() > 1) {
                    util::buffers::buffers_copy(
                        boost::asio::buffer(panel_buffer_, panel_size_), 
                        sample.data);
                    data = panel_buffer_;
                }
                if (transfer_) {
                    transfer_(picture_, data);
                }
                for (int i = 0; i < pic_in_.img.i_plane; ++i) {
                    pic_in_.img.plane[i] = data;
                    data += picture_.plane_sizes[i];
                }
                x264_encoder_encode(x264_, &p_nal_, &i_nal_, &pic_in_, &pic_out_);

                return true;
            }

            virtual bool push(
                Transcoder::eos_t const & eos, 
                boost::system::error_code & ec)
            {
                if (x264_encoder_delayed_frames(x264_) == 0) {
                    ec = boost::asio::error::eof;
                    return false;
                }
                x264_encoder_encode(x264_, &p_nal_, &i_nal_, NULL, &pic_out_);
                return true;
            }

            bool pop(
                Sample & sample, 
                boost::system::error_code & ec)
            {
                if (p_nal_) {
                    if (pic_out_.b_keyframe)
                        sample.flags |= sample.f_sync;
                    sample.time = (intptr_t)pic_out_.opaque;
                    sample.dts = pic_out_.i_dts;
                    sample.cts_delta = boost::uint32_t(pic_out_.i_pts - pic_out_.i_dts);
                    sample.size = 0;
                    sample.data.clear();
                    for (int i = 0; i < i_nal_; ++i) {
                        sample.data.push_back(boost::asio::buffer(p_nal_[i].p_payload, p_nal_[i].i_payload));
                        sample.size += p_nal_[i].i_payload;
                    }
                    std::vector<NaluBuffer> nalus;
                    for (int i = 0; i < i_nal_; ++i) {
                        nalus.push_back(NaluBuffer(
                            p_nal_[i].i_payload - 4, 
                            NaluBuffer::BuffersPosition(sample.data.begin() + i, sample.data.end(), 4), 
                            NaluBuffer::BuffersPosition(sample.data.begin() + (i + 1))));
                    }
                    nalus_.nalus(nalus);
                    sample.context = &nalus_;
                    p_nal_ = NULL;
                    return true;
                } else {
                    ec = boost::asio::error::would_block;
                    return false;
                }
            }

            bool refresh(
                boost::system::error_code & ec)
            {
                x264_encoder_intra_refresh(x264_);
                return true;
            }

            bool close(
                boost::system::error_code & ec)
            {
                x264_encoder_close(x264_);
                x264_ = NULL;

                pic_in_.img.plane[0] = panel_buffer_;
                x264_picture_clean(&pic_in_);

                return true;
            }

        private:
            x264_t * x264_;
            x264_param_t param_;
            x264_picture_t pic_in_;
            uint8_t * panel_buffer_;
            int panel_size_;
            x264_picture_t pic_out_;
            x264_nal_t * p_nal_;
            int i_nal_;

            ColorSpace::PictureSize picture_;
            ColorSpace::transfer_t transfer_;
            AvcConfigHelper config_;
            AvcNaluHelper nalus_;
        };

    } // namespace avcodec
} // namespace ppbox
