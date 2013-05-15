// AvcType.cpp

#include "ppbox/avcodec/Common.h"
#include "ppbox/avcodec/avc/AvcType.h"

namespace ppbox
{
    namespace avcodec
    {

       char const * const SliceHeader::slice_type_str[] = {
           "P", "B", "I", "SP", "SI", 
           "P2", "B2", "I2", "SP2", "SI2", 
       };

       char const * const NaluHeader::nalu_type_str[] = {
           "Unspecified", "non-IDR", "Slice A", "Slice B", "Slice C", 
           "IDR", "SEI", "SPS", "PPS", "AUD", "End Sequence", "End Stream", "Filler", 
       };

    } // namespace avcodec
} // namespace ppbox
