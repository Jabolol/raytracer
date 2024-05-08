#include <chrono>
#include "Common.hpp"
#include "core/Ray.hpp"
#include "interfaces/IHittable.hpp"
#include "utils/VecN.hpp"

#ifndef __CAMERA_HPP__
    #define __CAMERA_HPP__

namespace Raytracer::Core
{
    class Camera {
      private:
        double _aspectRatio = 1.0;
        int _imageWidth = 100;
        int _samplesPerPixel = 10;
        int _maxDepth = 10;
        Utils::Color _backgroundColor = Utils::Color(0, 0, 0);

        double _vFov = 90;
        Utils::Point3 _lookFrom = Utils::Point3(0, 0, 0);
        Utils::Point3 _lookAt = Utils::Point3(0, 0, -1);
        Utils::Vec3 _vUp = Utils::Vec3(0, 1, 0);

        double _defocusAngle = 0;
        double _focusDistance = 10;

        int _imageHeight;
        double _pixelSampleScale;

        Utils::Point3 _center;
        Utils::Point3 _pixelZeroLoc;
        Utils::Vec3 _pixelDeltaU;
        Utils::Vec3 _pixelDeltaV;
        Utils::Vec3 _u, _v, _w;
        Utils::Vec3 _defocusDiskU;
        Utils::Vec3 _defocusDiskV;

      public:
        Camera() = default;
        void setup();
        void render(const Interfaces::IHittable &world);
        Core::Ray getRay(double u, double v) const;
        Utils::Vec3 sampleSquare() const;
        Utils::Vec3 sampleDisk(double radius) const;
        Utils::Vec3 sampleDefocusDisk() const;
        Utils::Color rayColor(const Ray ray, int depth,
            const Interfaces::IHittable &world) const;
        void progress(
            const std::chrono::steady_clock::time_point &start, int j) const;
        GET_SET(double, aspectRatio)
        GET_SET(int, imageWidth)
        GET_SET(int, samplesPerPixel)
        GET_SET(int, maxDepth)
        GET_SET(Utils::Color, backgroundColor)
        GET_SET(double, vFov)
        GET_SET(Utils::Point3, lookFrom)
        GET_SET(Utils::Point3, lookAt)
        GET_SET(Utils::Vec3, vUp)
        GET_SET(double, defocusAngle)
        GET_SET(double, focusDistance)
        GET_SET(int, imageHeight)
        GET_SET(double, pixelSampleScale)
        GET_SET(Utils::Point3, center)
        GET_SET(Utils::Vec3, pixelZeroLoc)
        GET_SET(Utils::Vec3, pixelDeltaU)
        GET_SET(Utils::Vec3, pixelDeltaV)
        GET_SET(Utils::Vec3, u)
        GET_SET(Utils::Vec3, v)
        GET_SET(Utils::Vec3, w)
        GET_SET(Utils::Vec3, defocusDiskU)
        GET_SET(Utils::Vec3, defocusDiskV)
    };
} // namespace Raytracer::Core

#endif /* __CAMERA_HPP__ */
