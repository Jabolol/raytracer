#include <memory>
#include "Common.hpp"
#include "arguments/Kinds.hpp"
#include "interfaces/IArguments.hpp"
#include "interfaces/ITexture.hpp"
#include "utils/VecN.hpp"

#ifndef __ARG_MATERIALS_HPP__
    #define __ARG_MATERIALS_HPP__

namespace Raytracer::Arguments
{
    class Lambertian : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        Utils::Vec3 _color;
        std::shared_ptr<Interfaces::ITexture> _texture = nullptr;

      public:
        Lambertian(Utils::Vec3 color) : _color(color)
        {
            _kind = ArgumentKind::ARG_LAMBERTIAN_COLOR;
        }
        Lambertian(double r, double g, double b) : _color(r, g, b)
        {
            _kind = ArgumentKind::ARG_LAMBERTIAN_TEXTURE;
        }
        Lambertian(std::shared_ptr<Interfaces::ITexture> texture)
            : _texture(texture)
        {
            _kind = ArgumentKind::ARG_LAMBERTIAN_TEXTURE;
        }
        GET_SET(Utils::Vec3, color);
        GET_SET(std::shared_ptr<Interfaces::ITexture>, texture);
        ARG_KIND(_kind);
    };

    class Dielectric : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        double _refractionIndex;
        Utils::Color _color;

      public:
        Dielectric(double refractionIndex) : _refractionIndex(refractionIndex)
        {
            _kind = ArgumentKind::ARG_DIELECTRIC;
        }
        Dielectric(double refractionIndex, Utils::Color color)
            : _refractionIndex(refractionIndex)
        {
            _kind = ArgumentKind::ARG_DIELECTRIC_COLOR;
        }
        GET_SET(double, refractionIndex);
        GET_SET(Utils::Color, color);
        ARG_KIND(_kind);
    };

    class DiffuseLight : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        Utils::Vec3 _color;
        std::shared_ptr<Interfaces::ITexture> _texture = nullptr;

      public:
        DiffuseLight(Utils::Vec3 color) : _color(color)
        {
            _kind = ArgumentKind::ARG_DIFFUSE_LIGHT_COLOR;
        }
        DiffuseLight(std::shared_ptr<Interfaces::ITexture> texture)
            : _texture(texture)
        {
            _kind = ArgumentKind::ARG_DIFFUSE_LIGHT_TEXTURE;
        }
        GET_SET(Utils::Vec3, color);
        GET_SET(std::shared_ptr<Interfaces::ITexture>, texture);
        ARG_KIND(_kind);
    };

    class Isotropic : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        Utils::Color _color;
        std::shared_ptr<Interfaces::ITexture> _texture = nullptr;

      public:
        Isotropic(Utils::Color color) : _color(color)
        {
            _kind = ArgumentKind::ARG_ISOTROPIC_COLOR;
        }
        Isotropic(std::shared_ptr<Interfaces::ITexture> texture)
            : _texture(texture)
        {
            _kind = ArgumentKind::ARG_ISOTROPIC_TEXTURE;
        }
        GET_SET(Utils::Color, color);
        GET_SET(std::shared_ptr<Interfaces::ITexture>, texture);
        ARG_KIND(_kind);
    };

    class Metal : public Interfaces::IArguments {
      private:
        Utils::Vec3 _color;
        double _fuzz;

      public:
        Metal(Utils::Vec3 color, double fuzz) : _color(color), _fuzz(fuzz)
        {
        }
        GET_SET(Utils::Vec3, color);
        GET_SET(double, fuzz);
        ARG_KIND(ArgumentKind::ARG_METAL);
    };
} // namespace Raytracer::Arguments

#endif /* __ARG_MATERIALS_HPP__ */
