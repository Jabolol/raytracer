#include <string>
#include "exceptions/Base.hpp"

#ifndef __FILE_EXCEPTION_HPP__
    #define __FILE_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class FileException : public Base {
      public:
        FileException(const std::string &message)
            : Base("File error: " + message)
        {
        }
        virtual ~FileException() = default;
    };
} // namespace Raytracer::Exceptions

#endif /* __FILE_EXCEPTION_HPP__ */
