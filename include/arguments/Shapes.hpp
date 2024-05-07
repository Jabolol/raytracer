#include <memory>
#include "Common.hpp"
#include "arguments/Kinds.hpp"
#include "interfaces/IArguments.hpp"
#include "interfaces/IMaterial.hpp"
#include "interfaces/ITexture.hpp"
#include "utils/VecN.hpp"

#ifndef __ARG_SHAPES_HPP__
    #define __ARG_SHAPES_HPP__

namespace Raytracer::Arguments
{
    class Cone : public Interfaces::IArguments {
      private:
        double _radius;
        double _height;
        Utils::Point3 _center;
        std::shared_ptr<Interfaces::IMaterial> _material = nullptr;

      public:
        Cone(Utils::Point3 &center, double radius, double height,
            std::shared_ptr<Interfaces::IMaterial> material)
            : _radius(radius), _height(height), _center(center),
              _material(material)
        {
        }
        GET_SET(Utils::Point3, center);
        GET_SET(double, radius);
        GET_SET(double, height);
        GET_SET(std::shared_ptr<Interfaces::IMaterial>, material);
        ARG_KIND(ArgumentKind::ARG_CONE);
    };

    class Cylinder : public Interfaces::IArguments {
      private:
        double _radius;
        double _height;
        Utils::Point3 _center;
        std::shared_ptr<Interfaces::IMaterial> _material = nullptr;

      public:
        Cylinder(Utils::Point3 &center, double radius, double height,
            std::shared_ptr<Interfaces::IMaterial> material)
            : _radius(radius), _height(height), _center(center),
              _material(material)
        {
        }
        GET_SET(Utils::Point3, center);
        GET_SET(double, radius);
        GET_SET(double, height);
        GET_SET(std::shared_ptr<Interfaces::IMaterial>, material);
        ARG_KIND(ArgumentKind::ARG_CYLINDER);
    };

    class Plane : public Interfaces::IArguments {
      private:
        Utils::Point3 _point;
        Utils::Vec3 _normal;
        std::shared_ptr<Interfaces::IMaterial> _material = nullptr;

      public:
        Plane(Utils::Point3 &point, Utils::Vec3 &normal,
            std::shared_ptr<Interfaces::IMaterial> material)
            : _point(point), _normal(normal), _material(material)
        {
        }
        GET_SET(Utils::Point3, point);
        GET_SET(Utils::Vec3, normal);
        GET_SET(std::shared_ptr<Interfaces::IMaterial>, material);
        ARG_KIND(ArgumentKind::ARG_PLANE);
    };

    class Quad : public Interfaces::IArguments {
      private:
        Utils::Point3 _Q;
        Utils::Point3 _u;
        Utils::Point3 _v;
        std::shared_ptr<Interfaces::IMaterial> _material = nullptr;

      public:
        Quad(Utils::Point3 &Q, Utils::Point3 &u, Utils::Point3 &v,
            std::shared_ptr<Interfaces::IMaterial> material)
            : _Q(Q), _u(u), _v(v), _material(material)
        {
        }
        GET_SET(Utils::Point3, Q);
        GET_SET(Utils::Point3, u);
        GET_SET(Utils::Point3, v);
        GET_SET(std::shared_ptr<Interfaces::IMaterial>, material);
        ARG_KIND(ArgumentKind::ARG_QUAD);
    };

    class Sphere : public Interfaces::IArguments {
      private:
        ArgumentKind _kind;
        Utils::Point3 _center;
        Utils::Point3 _centerTwo;
        double _radius;
        std::shared_ptr<Interfaces::IMaterial> _material = nullptr;

      public:
        Sphere(Utils::Point3 center, double radius,
            std::shared_ptr<Interfaces::IMaterial> material)
            : _center(center), _radius(radius), _material(material)
        {
            _kind = ArgumentKind::ARG_SPHERE;
        }
        Sphere(Utils::Point3 centerOne, Utils::Point3 centerTwo, double radius,
            std::shared_ptr<Interfaces::IMaterial> material)
            : _center(centerOne), _centerTwo(centerTwo), _radius(radius),
              _material(material)
        {
            _kind = ArgumentKind::ARG_SPHERE_MOVING;
        }
        GET_SET(Utils::Point3, center);
        GET_SET(Utils::Point3, centerTwo);
        GET_SET(double, radius);
        GET_SET(std::shared_ptr<Interfaces::IMaterial>, material);
        ARG_KIND(_kind);
    };
} // namespace Raytracer::Arguments

#endif /* __ARG_SHAPES_HPP__ */
