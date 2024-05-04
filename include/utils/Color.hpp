#include "VecN.hpp"

#ifndef __COLOR_HPP__
    #define __COLOR_HPP__

namespace Raytracer::Utils
{
    double linearToGamma(double linear);
    void writeColor(std::ostream &out, const Color &pixelColor);
} // namespace Raytracer::Utils

#endif /* __COLOR_HPP__ */
