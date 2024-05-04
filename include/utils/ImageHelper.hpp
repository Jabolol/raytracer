#include <string>
#include <vector>
#include "Common.hpp"

#ifndef __IMAGE_HELPER_HPP__
    #define __IMAGE_HELPER_HPP__

namespace Raytracer
{
    namespace Utils
    {
        class ImageHelper {
          private:
            int _width = 0;
            int _height = 0;
            std::vector<unsigned char> data;

          public:
            ImageHelper() = default;
            ImageHelper(const char *filename);
            bool load(const std::string &filename);
            const unsigned char *pixelData(int x, int y) const;
            GET_SET(int, width);
            GET_SET(int, height);
        };
    } // namespace Utils
} // namespace Raytracer

#endif /* __IMAGE_HELPER_HPP__ */
