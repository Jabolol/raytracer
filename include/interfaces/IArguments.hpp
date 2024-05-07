#include "arguments/Kinds.hpp"

#ifndef __IARGUMENTS_HPP__
    #define __IARGUMENTS_HPP__

namespace Raytracer::Interfaces
{
    class IArguments {
      public:
        virtual ~IArguments() = default;
        virtual Arguments::ArgumentKind kind() const = 0;
    };
} // namespace Raytracer::Interfaces

#endif /* __IARGUMENTS_HPP__ */
