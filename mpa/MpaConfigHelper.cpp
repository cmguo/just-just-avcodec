// MpaConfigHelper.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/mpa/MpaConfigHelper.h"
#include "ppbox/avcodec/mpa/MpaConfig.h"

#include <ppbox/avbase/stream/BitsIStream.h>
#include <ppbox/avbase/stream/BitsOStream.h>
#include <ppbox/avbase/stream/FormatBuffer.h>
#include <ppbox/avbase/StreamInfo.h>
using namespace ppbox::avbase;

namespace ppbox
{
    namespace avcodec
    {

#undef free
#define free MpaConfigHelper::bitrate_free
#undef bad
#define bad MpaConfigHelper::bitrate_bad

        boost::uint32_t const MpaConfigHelper::bitrate_table[2][4][16] = // bitrate_table[version][layer][index]
        {
            { // mpeg 2
                {0}, // reserved
                {free,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, bad}, // layer 3
                {free,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, bad}, // layer 2
                {free,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, bad}, // layer 1
            }, 
            { // mpeg 1
                {0}, // reserved
                {free,  32,  40,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, bad}, // layer 3
                {free,  32,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, bad}, // layer 2
                {free,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, bad}, // layer 1
            }, 
        };

        boost::uint32_t const MpaConfigHelper::frequency_table[2][4] = // frequency_table[version][index]
        {
            //{11025, 12000, 8000, 0}, // mpeg 2.5
            //{}, // reserved
            {22050, 24000, 16000, 0}, // mpeg 2
            {44100, 48000, 32000, 0}, // mpeg 1
        };

        /* Channel Mode
         * 00 - Stereo
         * 01 - Joint stereo (Stereo)
         * 10 - Dual channel (Stereo)
         * 11 - Single channel (Mono)
         */
        boost::uint32_t const MpaConfigHelper::channel_count_table[4] = // channel_count_table[mode]
        {
            2, 2, 2, 1, 
        };

        boost::uint32_t const MpaConfigHelper::sample_per_frame_table[2][4] = // sample_per_frame_table[version][layer]
        {
            // reserved layer3 layer 2 layer 1
            //{0, 576, 1152, 384}, // mpeg 2.5
            //{}, // reserved
            {0, 576, 1152, 384}, // mpeg 2
            {0, 1152, 1152, 384}, // mpeg 1
        };

        MpaConfigHelper::MpaConfigHelper()
            : data_(new MpaConfig)
        {
        }

        MpaConfigHelper::MpaConfigHelper(
            boost::uint8_t const * buf, 
            boost::uint32_t size)
            : data_(new MpaConfig)
        {
            std::vector<boost::uint8_t> vec(buf, buf + size);
            from_data(vec);
        }

        MpaConfigHelper::MpaConfigHelper(
            std::vector<boost::uint8_t> const & vec)
            : data_(new MpaConfig)
        {
            from_data(vec);
        }

        MpaConfigHelper::~MpaConfigHelper()
        {
            delete data_;
        }

        MpaConfigHelper::VersionEnum MpaConfigHelper::get_version() const
        {
            return (VersionEnum)(boost::uint32_t)data_->version;
        }

        MpaConfigHelper::LayerEnum MpaConfigHelper::get_layer() const
        {
            return (LayerEnum)(boost::uint32_t)data_->layer;
        }

        boost::uint32_t MpaConfigHelper::get_bitrate() const
        {
            return bitrate_table[data_->version][data_->layer][data_->bitrate_index] * 1000;
        }

        boost::uint32_t MpaConfigHelper::get_frequency() const
        {
            return frequency_table[data_->version][data_->frequency_index];
        }

        boost::uint32_t MpaConfigHelper::get_channel_count() const
        {
            return channel_count_table[data_->channel_mode];
        }

        boost::uint32_t MpaConfigHelper::get_sample_per_frame() const
        {
            return sample_per_frame_table[data_->version][data_->layer];
        }

        /* How to calculate frame length
         * For Layer I files us this formula:
         *   FrameLengthInBytes = (12 * BitRate / SampleRate + Padding) * 4
         * For Layer II & III files use this formula:
         *   FrameLengthInBytes = 144 * BitRate / SampleRate + Padding
         */

        static boost::uint32_t slot_size_table[] = {
            // reserved layer3 layer 2 layer 1
            0, 1, 1, 4
        };

        boost::uint32_t MpaConfigHelper::get_frame_size() const
        {
            return (boost::uint32_t)((boost::uint64_t)get_bitrate() * get_sample_per_frame() / get_frequency() / 8)
                + (data_->padding_bit ? slot_size_table[get_layer()] : 0);
        }

        void MpaConfigHelper::set_version(
            VersionEnum version)
        {
            data_->version = version;
        }

        void MpaConfigHelper::set_layer(
            LayerEnum layer)
        {
            data_->layer = layer;
        }

        void MpaConfigHelper::set_bitrate(
            boost::uint32_t bitrate)
        {
            boost::uint32_t const (&bitrates)[16] = bitrate_table[data_->version][data_->layer];
            bitrate /= 1000;
            for (size_t i = 0; i < sizeof(bitrates) / sizeof(bitrates[0]); ++i) {
                if (bitrate == bitrates[i]) {
                    data_->bitrate_index = i;
                    return;
                }
            }
        }

        void MpaConfigHelper::set_frequency(
            boost::uint32_t frequency)
        {
            boost::uint32_t const (&frequencys)[4] = frequency_table[data_->version];
            for (size_t i = 0; i < sizeof(frequencys) / sizeof(frequencys[0]); ++i) {
                if (frequency == frequencys[i]) {
                    data_->frequency_index = i;
                    return;
                }
            }
        }

        void MpaConfigHelper::set_channel_count(
            boost::uint32_t channel_count)
        {
            data_->channel_mode = channel_count == 1 ? 3 : 0;
        }

        void MpaConfigHelper::from_data(
            std::vector<boost::uint8_t> const & buf)
        {
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size(), buf.size());
            BitsIStream<boost::uint8_t> is(abuf);
            is >> *data_;
        }

        void MpaConfigHelper::to_data(
            std::vector<boost::uint8_t> & buf) const
        {
            buf.resize(16);
            FormatBuffer abuf((boost::uint8_t *)&buf[0], buf.size());
            BitsOStream<boost::uint8_t> os(abuf);
            os << *data_;
            while (!os.byte_aligned())
                os << U<1>(0);
            buf.resize(abuf.size());
        }

        bool MpaConfigHelper::ready() const
        {
            return data_->layer != 0;
        }

        void MpaConfigHelper::set_format(
            AudioInfo const & info)
        {
            if (!ready()) {
                return;
            }
            set_channel_count(info.channel_count);
            data_->frequency_index = 3;
            set_frequency(info.sample_rate);
            if (data_->frequency_index == 3) {
                data_->version =  1 - data_->version;
                set_frequency(info.sample_rate);
            }
            if (data_->layer == 0 && data_->frequency_index != 3) { // we can say version is correct now
                boost::uint32_t const (&sample_per_frames)[4] = sample_per_frame_table[data_->version];
                for (size_t i = 0; i < sizeof(sample_per_frames) / sizeof(sample_per_frames[0]); ++i) {
                    if (info.sample_per_frame == sample_per_frames[i]) {
                        data_->layer = i;
                        break;
                    }
                }
            }
        }

        void MpaConfigHelper::get_format(
            AudioInfo & info) const
        {
            if (!ready()) {
                return;
            }
            info.channel_count = get_channel_count();
            info.sample_rate = get_frequency();
            info.sample_per_frame = get_sample_per_frame();
        }

    } // namespace avcodec
} // namespace ppbox
