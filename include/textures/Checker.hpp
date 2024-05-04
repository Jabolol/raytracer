#include <memory>
#include "interfaces/ITexture.hpp"

#ifndef __CHECKER_HPP__
    #define __CHECKER_HPP__

namespace Raytracer
{
    namespace Textures
    {
        class Checker : public Interfaces::ITexture {
          private:
            std::shared_ptr<Interfaces::ITexture> _odd;
            std::shared_ptr<Interfaces::ITexture> _even;
            double _scale;

          public:
            Checker(double scale, std::shared_ptr<Interfaces::ITexture> even,
                std::shared_ptr<Interfaces::ITexture> odd);
            Checker(
                double scale, const Utils::Color &a, const Utils::Color &b);
            Utils::Color value(
                double u, double v, const Utils::Point3 &point) const override;
        };
    } // namespace Textures
} // namespace Raytracer

#endif /* __CHECKER_HPP__ */
