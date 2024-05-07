#include <memory>
#include "Common.hpp"
#include "arguments/Kinds.hpp"
#include "interfaces/IArguments.hpp"
#include "interfaces/ITexture.hpp"
#include "utils/VecN.hpp"

#ifndef __ARG_TEXTURES_HPP__
    #define __ARG_TEXTURES_HPP__

namespace Raytracer::Arguments
{
    class Solid : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        Utils::Vec3 _color;

      public:
        Solid(Utils::Vec3 color) : _color(color)
        {
            _kind = ArgumentKind::ARG_SOLID_COLOR;
        }
        Solid(double r, double g, double b) : _color(r, g, b)
        {
            _kind = ArgumentKind::ARG_SOLID_RGB;
        }
        GET_SET(Utils::Vec3, color);
        ARG_KIND(_kind);
    };

    class Noise : public Interfaces::IArguments {
      private:
        double _scale;

      public:
        Noise(double scale) : _scale(scale)
        {
        }
        GET_SET(double, scale);
        ARG_KIND(ArgumentKind::ARG_NOISE);
    };

    class Image : public Interfaces::IArguments {
      private:
        std::string _filename;

      public:
        Image(std::string filename) : _filename(filename)
        {
        }
        GET_SET(std::string, filename);
        ARG_KIND(ArgumentKind::ARG_IMAGE);
    };

    class Checker : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        double _scale;
        Utils::Vec3 _color1;
        Utils::Vec3 _color2;
        std::shared_ptr<Interfaces::ITexture> _texture1 = nullptr;
        std::shared_ptr<Interfaces::ITexture> _texture2 = nullptr;

      public:
        Checker(double scale, Utils::Vec3 color1, Utils::Vec3 color2)
            : _scale(scale), _color1(color1), _color2(color2)
        {
            _kind = ArgumentKind::ARG_CHECKER_COLOR;
        }
        Checker(double scale, std::shared_ptr<Interfaces::ITexture> texture1,
            std::shared_ptr<Interfaces::ITexture> texture2)
            : _scale(scale), _texture1(texture1), _texture2(texture2)
        {
            _kind = ArgumentKind::ARG_CHECKER_TEXTURE;
        }
        GET_SET(double, scale);
        GET_SET(Utils::Vec3, color1);
        GET_SET(Utils::Vec3, color2);
        GET_SET(std::shared_ptr<Interfaces::ITexture>, texture1);
        GET_SET(std::shared_ptr<Interfaces::ITexture>, texture2);
        ARG_KIND(_kind);
    };
} // namespace Raytracer::Arguments

#endif /* __ARG_TEXTURES_HPP__ */
