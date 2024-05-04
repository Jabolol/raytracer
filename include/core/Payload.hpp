#include <memory>
#include "core/Ray.hpp"
#include "interfaces/IMaterial.hpp"
#include "utils/VecN.hpp"

#ifndef __PAYLOAD_HPP__
    #define __PAYLOAD_HPP__

namespace Raytracer::Core
{
    class Payload {
      private:
        Utils::Point3 _point;
        Utils::Vec3 _normal;
        std::shared_ptr<Interfaces::IMaterial> _material;
        double _t;
        double _u;
        double _v;
        bool _frontFace;

      public:
        Payload() = default;
        void setFaceNormal(
            const Core::Ray &ray, const Utils::Vec3 &outwardNormal);
        GET_SET(Utils::Point3, point)
        GET_SET(Utils::Vec3, normal)
        GET_SET(std::shared_ptr<Interfaces::IMaterial>, material)
        GET_SET(double, t)
        GET_SET(double, u)
        GET_SET(double, v)
        GET_SET(bool, frontFace)
    };
} // namespace Raytracer::Core

#endif /* __PAYLOAD_HPP__ */
