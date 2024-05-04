#include "VecN.hpp"

#ifndef __COLOR_HPP__
    #define __COLOR_HPP__

namespace Raytracer
{
    namespace Utils
    {
        double linearToGamma(double linear);
        void writeColor(std::ostream &out, const Color &pixelColor);
    } // namespace Utils
} // namespace Raytracer

#endif /* __COLOR_HPP__ */
