#include <string>
#include "exceptions/Base.hpp"

#ifndef __PARSE_EXCEPTION_HPP__
    #define __PARSE_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class ParseException : public Base {
      public:
        ParseException(const std::string &message)
            : Base("Parse error: " + message)
        {
        }
        virtual ~ParseException() = default;
    };
} // namespace Raytracer::Exceptions

#endif /* __PARSE_EXCEPTION_HPP__ */
