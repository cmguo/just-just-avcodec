// AvcAgorithm.h

#ifndef _PPBOX_AVCODEC_AVC_AVC_AGORITHM_H_
#define _PPBOX_AVCODEC_AVC_AVC_AGORITHM_H_

#include "ppbox/avcodec/BitsType.h"

namespace ppbox
{
    namespace avcodec
    {

        template <
            typename V
        >
        struct DirectValue
        {
            DirectValue(
                V const & v)
                : v_(&v)
            {
            }

            V const & operator()() const
            {
                return *v_;
            }

        private:
            V const * v_;
        };

        template <
            typename V
        >
        inline DirectValue<V> const direct_value(
            V const & v)
        {
            return DirectValue<V>(v);
        }

        template <
            typename Object, 
            typename Result
        >
        struct ConstMemFunc
        {
            ConstMemFunc(
                Object const * obj, 
                Result (Object::*func)() const)
                : obj_(obj)
                , func_(func)
            {
            }

            Result operator()() const
            {
                return (obj_->*func_)();
            }

        private:
            Object const * obj_;
            Result (Object::* func_)() const;
        };

        template <
            typename Object, 
            typename Result
        >
        inline ConstMemFunc<Object, Result> const mem_func(
            Object const * obj, 
            Result (Object::*func)() const)
        {
            return ConstMemFunc<Object, Result>(obj, func);
        }

    } // namespace avcodec
} // namespace ppbox

#endif // _PPBOX_AVCODEC_AVC_AVC_AGORITHM_H_
