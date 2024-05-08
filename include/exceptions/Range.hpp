#include <string>
#include "exceptions/Base.hpp"

#ifndef __RANGE_EXCEPTION_HPP__
    #define __RANGE_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class RangeException : public Base {
      public:
        RangeException(const std::string &message)
            : Base("Range error: " + message)
        {
        }
        virtual ~RangeException() = default;
    };
} // namespace Raytracer::Exceptions

#endif /* __RANGE_EXCEPTION_HPP__ */
