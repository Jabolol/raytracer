#include "core/Camera.hpp"
#include "interfaces/IHittable.hpp"
#include "utils/Color.hpp"
#include "utils/VecN.hpp"

void Raytracer::Core::Camera::setup()
{
    _imageHeight = static_cast<int>(_imageWidth / _aspectRatio);
    _imageHeight = (_imageHeight < 1) ? 1 : _imageHeight;

    _pixelSampleScale = 1.0 / _samplesPerPixel;

    _center = _lookFrom;

    double theta = Utils::degreesToRadians(_vFov);
    double h = tan(theta / 2);
    double viewportHeight = 2 * h * _focusDistance;
    double viewportWidth =
        viewportHeight * (static_cast<double>(_imageWidth) / _imageHeight);

    _w = Utils::unitVector(_lookFrom - _lookAt);
    _u = Utils::unitVector(Utils::cross(_vUp, _w));
    _v = Utils::cross(_w, _u);

    Utils::Vec3 viewportU = viewportWidth * _u;
    Utils::Vec3 viewportV = viewportHeight * -_v;

    _pixelDeltaU = viewportU / _imageWidth;
    _pixelDeltaV = viewportV / _imageHeight;

    Utils::Vec3 viewportUL =
        _center - (_focusDistance * _w) - viewportU / 2 - viewportV / 2;
    _pixelZeroLoc = viewportUL + 0.5 * (_pixelDeltaU + _pixelDeltaV);

    double defocusRadius =
        _focusDistance * std::tan(Utils::degreesToRadians(_defocusAngle / 2));
    _defocusDiskU = _u * defocusRadius;
    _defocusDiskV = _v * defocusRadius;
}

void Raytracer::Core::Camera::render(const Interfaces::IHittable &world)
{
    setup();

    std::cout << "P3\n" << _imageWidth << ' ' << _imageHeight << "\n255\n";

    auto start = std::chrono::steady_clock::now();

    for (int j = 0; j < _imageHeight; j++) {
        progress(start, j);

        for (int i = 0; i < _imageWidth; i++) {
            Utils::Color pixelColor = Utils::Color(0, 0, 0);

            for (int s = 0; s < _samplesPerPixel; s++) {
                Ray ray = getRay(i, j);
                pixelColor += rayColor(ray, _maxDepth, world);
            }
            Utils::writeColor(std::cout, _pixelSampleScale * pixelColor);
        }
    }
}

Raytracer::Core::Ray Raytracer::Core::Camera::getRay(double i, double j) const
{
    Utils::Vec3 offset = sampleSquare();
    Utils::Vec3 sample = _pixelZeroLoc + ((i + offset.x()) * _pixelDeltaU)
        + ((j + offset.y()) * _pixelDeltaV);

    Utils::Point3 origin =
        (_defocusAngle <= 0) ? _center : sampleDefocusDisk();
    Utils::Vec3 direction = sample - origin;
    double time = Utils::randomDouble();

    return Ray(origin, direction, time);
}

Raytracer::Utils::Vec3 Raytracer::Core::Camera::sampleSquare() const
{
    return Utils::Vec3(
        Utils::randomDouble() - 0.5, Utils::randomDouble() - 0.5, 0);
}

Raytracer::Utils::Vec3 Raytracer::Core::Camera::sampleDisk(double radius) const
{
    return radius * Utils::randomInUnitDisk<double, 3>();
}

Raytracer::Utils::Vec3 Raytracer::Core::Camera::sampleDefocusDisk() const
{
    Utils::Vec3 p = Utils::randomInUnitDisk<double, 3>();

    return _center + (p.x() * _defocusDiskU) + (p.y() * _defocusDiskV);
}

Raytracer::Utils::Color Raytracer::Core::Camera::rayColor(
    const Ray ray, int depth, const Interfaces::IHittable &world) const
{
    if (depth <= 0) {
        return Utils::Color(0, 0, 0);
    }

    Payload payload;
    double infinity = std::numeric_limits<double>::infinity();

    if (!world.hit(ray, Utils::Interval(0.001, infinity), payload)) {
        return _backgroundColor;
    }

    Ray scattered;
    Utils::Color attenuation;
    Utils::Color emissionColor =
        payload.material()->emitted(payload.u(), payload.v(), payload.point());

    if (!payload.material()->scatter(ray, payload, attenuation, scattered)) {
        return emissionColor;
    }

    Utils::Color scatterColor =
        attenuation * rayColor(scattered, depth - 1, world);

    return emissionColor + scatterColor;
}

void Raytracer::Core::Camera::progress(
    const std::chrono::steady_clock::time_point &start, int j) const
{
    std::clog << "\rProgress: [";
    std::clog << "\033[36m";
    for (int p = 0; p < 50; ++p) {
        if (p * 100 / 50 <= j * 100 / _imageHeight)
            std::clog << "#";
        else
            std::clog << " ";
    }
    std::clog << "\033[0m";
    std::clog << "] " << j * 100 / _imageHeight << "%" << " ("
              << std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "s)" << std::flush;
}
