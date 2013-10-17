// Error.h

#ifndef _PPBOX_AVCODEC_ERROR_H_
#define _PPBOX_AVCODEC_ERROR_H_

namespace ppbox
{
    namespace avcodec
    {

        namespace error {

            enum errors
            {
                codec_not_support = 1, 
                encoder_not_support, 
                deccoder_not_support, 
                capture_not_support, 
                assembler_not_support, 
                splitter_not_support, 
                debuger_not_support, 
            };

            namespace detail {

                class avcodec_category
                    : public boost::system::error_category
                {
                public:
                    const char* name() const
                    {
                        return "avcodec";
                    }

                    std::string message(int value) const
                    {
                        switch (value) 
                        {
                            case codec_not_support:
                                return "avcodec: codec not support";
                            case encoder_not_support:
                                return "avcodec: encoder not support";
                            case deccoder_not_support:
                                return "avcodec: deccoder not support";
                            case capture_not_support:
                                return "avcodec: capture not support";
                            case assembler_not_support:
                                return "avcodec: assembler not support";
                            case splitter_not_support:
                                return "avcodec: splitter not support";
                            case debuger_not_support:
                                return "avcodec: debuger not support";
                            default:
                                return "avcodec: other error";
                        }
                    }
                };

            } // namespace detail

            inline const boost::system::error_category & get_category()
            {
                static detail::avcodec_category instance;
                return instance;
            }

            inline boost::system::error_code make_error_code(
                errors e)
            {
                return boost::system::error_code(
                    static_cast<int>(e), get_category());
            }

        } // namespace live_error

    } // namespace live
} // namespace ppbox

namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<ppbox::avcodec::error::errors>
        {
            BOOST_STATIC_CONSTANT(bool, value = true);
        };

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
        using ppbox::avcodec::error::make_error_code;
#endif

    }
}

#endif // _PPBOX_AVCODEC_ERROR_H_
