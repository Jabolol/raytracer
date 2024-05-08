#include <exception>
#include <string>

#ifndef __BASE_EXCEPTION_HPP__
    #define __BASE_EXCEPTION_HPP__

namespace Raytracer::Exceptions
{
    class Base : public std::exception {
      public:
        Base(const std::string &message) : _message(message)
        {
        }
        virtual ~Base() = default;

        virtual const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message;
    };
} // namespace Raytracer::Exceptions

#endif /* __BASE_EXCEPTION_HPP__ */
