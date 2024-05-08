#include <string>
#include "exceptions/Base.hpp"

#ifndef __MISSING_EXCEPTION_HPP__
    #define __MISSING_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class MissingException : public Base {
      public:
        MissingException(const std::string &message)
            : Base("Missing error: " + message)
        {
        }
        virtual ~MissingException() = default;
    };
} // namespace Raytracer::Exceptions

#endif /* __MISSING_EXCEPTION_HPP__ */
