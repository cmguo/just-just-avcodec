// HevcConfig.h

#ifndef _PPBOX_HEVCODEC_HEVC_HEVC_CONFIG_H_
#define _PPBOX_HEVCODEC_HEVC_HEVC_CONFIG_H_

#include "ppbox/avcodec/BitsType.h"

#include <util/serialization/Collection.h>

namespace ppbox
{
    namespace avcodec
    {

        // HEVC Decoder Configuration Record

        struct HevcConfig
        {
        public:
            U<8> configurationVersion;
            U<2> general_profile_space;
            U<1> general_tier_flag;
            U<5> general_profile_idc;
            U<32> general_profile_compatibility_flags;
            U<48> general_constraint_indicator_flags;
            U<8> general_level_idc;
            U<4> reserved;
            U<12> min_spatial_segmentation_idc;
            U<6> reserved2;
            U<2> parallelismType;
            U<6> reserved3;
            U<2> chromaFormat;
            U<5> reserved4;
            U<3> bitDepthLumaMinus8;
            U<5> reserved5;
            U<3> bitDepthChromaMinus8;
            U<16> avgFrameRate;
            U<2> constantFrameRate;
            U<3> numTemporalLayers;
            U<1> temporalIdNested;
            U<2> lengthSizeMinusOne;
            U<8> numOfArrays;

            struct ArrayElem
            {
                U<1> array_completeness;
                U<1> reserved;
                U<6> NAL_unit_type;
                U<16> numNalus;
                // for (i=0; i< numNalus; i++)
                    std::vector<U<16> > nalUnitLength;
                    std::vector<std::vector<boost::uint8_t> > nalUnit;

                template <
                    typename Archive
                >
                void serialize(
                    Archive & ar)
                {
                    ar & array_completeness;
                    ar & reserved;
                    ar & NAL_unit_type;
                    ar & numNalus;
                    nalUnitLength.resize(numNalus);
                    nalUnit.resize(numNalus);
                    for (size_t i = 0; i < nalUnit.size(); ++i) {
                        ar & nalUnitLength[i];
                        nalUnit[i].resize(nalUnitLength[i]);
                        util::serialization::serialize_collection(ar, nalUnit[i], nalUnit[i].size());
                    }
                }
            };

            // for (i=0; i< numOfArrays; i++)
                std::vector<ArrayElem> arrays;

        public:
            template <
                typename Archive
            >
            void serialize(
                Archive & ar)
            {
                ar & configurationVersion;
                ar & general_profile_space;
                ar & general_tier_flag;
                ar & general_profile_idc;
                ar & general_profile_compatibility_flags;
                ar & general_constraint_indicator_flags;
                ar & general_level_idc;
                ar & reserved;
                ar & min_spatial_segmentation_idc;
                ar & reserved2;
                ar & parallelismType;
                ar & reserved3;
                ar & chromaFormat;
                ar & reserved4;
                ar & bitDepthLumaMinus8;
                ar & reserved5;
                ar & bitDepthChromaMinus8;
                ar & avgFrameRate;
                ar & constantFrameRate;
                ar & numTemporalLayers;
                ar & temporalIdNested;
                ar & lengthSizeMinusOne;
                ar & numOfArrays;

                arrays.resize(numOfArrays);
                for (size_t i = 0; i < arrays.size(); ++i) {
                    ar & arrays[i];
                }
            }
        };

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_HEVCODEC_HEVC_HEVC_CONFIG_H_
