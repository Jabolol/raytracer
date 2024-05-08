#include <string>
#include "exceptions/Base.hpp"

#ifndef __ARGUMENT_EXCEPTION_HPP__
    #define __ARGUMENT_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class ArgumentException : public Base {
      public:
        ArgumentException(const std::string &message)
            : Base("Argument error: " + message)
        {
        }
        virtual ~ArgumentException() = default;
    };
} // namespace Raytracer::Exceptions

#endif /* __ARGUMENT_EXCEPTION_HPP__ */
