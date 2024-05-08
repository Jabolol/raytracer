#include <string>
#include "exceptions/Base.hpp"

#ifndef __CYCLIC_EXCEPTION_HPP__
    #define __CYCLIC_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class CyclicException : public Base {
      public:
        CyclicException(const std::string &message)
            : Base("Cyclic error: " + message)
        {
        }
        virtual ~CyclicException() = default;
    };
} // namespace Raytracer::Exceptions

#endif /* __CYCLIC_EXCEPTION_HPP__ */
