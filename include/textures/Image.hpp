#include "interfaces/ITexture.hpp"
#include "utils/ImageHelper.hpp"

#ifndef __IMAGE_HPP__
    #define __IMAGE_HPP__

namespace Raytracer::Textures
{
    class Image : public Interfaces::ITexture {
      private:
        Utils::ImageHelper _helper;

      public:
        Image(std::string filename);
        Utils::Color value(
            double u, double v, const Utils::Point3 &point) const override;
    };
} // namespace Raytracer::Textures

#endif /* __IMAGE_HPP__ */
