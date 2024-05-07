#include "Common.hpp"
#include "arguments/Kinds.hpp"
#include "interfaces/IArguments.hpp"
#include "interfaces/IHittable.hpp"
#include "interfaces/ITexture.hpp"
#include "utils/VecN.hpp"

#ifndef __ARG_EFFECTS_HPP__
    #define __ARG_EFFECTS_HPP__

namespace Raytracer::Arguments
{
    class RotateX : public Interfaces::IArguments {
      private:
        double _angle;
        std::shared_ptr<Interfaces::IHittable> _object = nullptr;

      public:
        RotateX(std::shared_ptr<Interfaces::IHittable> object, double angle)
            : _angle(angle), _object(object)
        {
        }
        GET_SET(double, angle);
        GET_SET(std::shared_ptr<Interfaces::IHittable>, object);
        ARG_KIND(ArgumentKind::ARG_ROTATE_X);
    };

    class RotateY : public Interfaces::IArguments {
      private:
        double _angle;
        std::shared_ptr<Interfaces::IHittable> _object = nullptr;

      public:
        RotateY(std::shared_ptr<Interfaces::IHittable> object, double angle)
            : _angle(angle), _object(object)
        {
        }
        GET_SET(double, angle);
        GET_SET(std::shared_ptr<Interfaces::IHittable>, object);
        ARG_KIND(ArgumentKind::ARG_ROTATE_Y);
    };

    class RotateZ : public Interfaces::IArguments {
      private:
        double _angle;
        std::shared_ptr<Interfaces::IHittable> _object = nullptr;

      public:
        RotateZ(std::shared_ptr<Interfaces::IHittable> object, double angle)
            : _angle(angle), _object(object)
        {
        }
        GET_SET(double, angle);
        GET_SET(std::shared_ptr<Interfaces::IHittable>, object);
        ARG_KIND(ArgumentKind::ARG_ROTATE_Z);
    };

    class Smoke : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        double _density;
        Utils::Color _color;
        std::shared_ptr<Interfaces::ITexture> _texture = nullptr;
        std::shared_ptr<Interfaces::IHittable> _object = nullptr;

      public:
        Smoke(std::shared_ptr<Interfaces::IHittable> object, double density,
            Utils::Color color)
            : _density(density), _color(color), _object(object)
        {
            _kind = ArgumentKind::ARG_SMOKE_COLOR;
        }
        Smoke(std::shared_ptr<Interfaces::IHittable> object, double density,
            std::shared_ptr<Interfaces::ITexture> texture)
            : _density(density), _texture(texture), _object(object)
        {
            _kind = ArgumentKind::ARG_SMOKE_TEXTURE;
        }
        GET_SET(double, density);
        GET_SET(Utils::Color, color);
        GET_SET(std::shared_ptr<Interfaces::ITexture>, texture);
        GET_SET(std::shared_ptr<Interfaces::IHittable>, object);
        ARG_KIND(_kind);
    };

    class Translate : public Interfaces::IArguments {
      private:
        Utils::Vec3 _offset;
        std::shared_ptr<Interfaces::IHittable> _object = nullptr;

      public:
        Translate(
            std::shared_ptr<Interfaces::IHittable> object, Utils::Vec3 offset)
            : _offset(offset)
        {
        }
        GET_SET(Utils::Vec3, offset);
        GET_SET(std::shared_ptr<Interfaces::IHittable>, object);
        ARG_KIND(ArgumentKind::ARG_TRANSLATE);
    };
} // namespace Raytracer::Arguments
#endif /* __ARG_EFFECTS_HPP__ */
