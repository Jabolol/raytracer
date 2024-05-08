#include "core/Camera.hpp"
#include "interfaces/IHittable.hpp"
#include "utils/Color.hpp"
#include "utils/VecN.hpp"

/**
 * @brief Set up the camera with the given parameters.
 *
 * This function calculates the image height based on the aspect ratio and the
 * image width. It also calculates the viewport height and width based on the
 * vertical field of view and the focus distance. The pixel sample scale is
 * calculated based on the number of samples per pixel. The camera's center
 * is set to the look from point. The camera's u, v, and w vectors are
 * calculated based on the look from, look at, and v up points. The viewport
 * u and v vectors are calculated based on the viewport width and height and
 * the u and v vectors. The pixel delta u and v vectors are calculated based
 * on the viewport u and v vectors and the image width and height. The pixel
 * zero location is calculated based on the viewport upper left corner and
 * the pixel delta u and v vectors. The defocus disk u and v vectors are
 * calculated based on the defocus angle and the u and v vectors.
 *
 * @return void
 */
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

/**
 * @brief Render the scene with the given camera.
 *
 * This function sets up the camera and prints the PPM header. It then loops
 * through each pixel in the image and calculates the pixel color based on the
 * number of samples per pixel. The pixel color is then written to the output
 * stream.
 *
 * @param world The world to render.
 * @return void
 */
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

/**
 * @brief Get the ray for the given pixel.
 *
 * This function calculates the sample location based on the pixel location
 * and the pixel delta u and v vectors. The origin is set to the center of the
 * camera if the defocus angle is less than or equal to 0. Otherwise, the
 * origin is set to a point on the defocus disk. The direction is set to the
 * sample location minus the origin. The time is set to a random double.
 *
 * @param i The x coordinate of the pixel.
 * @param j The y coordinate of the pixel.
 * @return The ray for the given pixel.
 */
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

/**
 * @brief Sample a square.
 *
 * This function returns a random point in a square centered at the origin
 * with a side length of 1.
 *
 * @return A random point in a square.
 */
Raytracer::Utils::Vec3 Raytracer::Core::Camera::sampleSquare() const
{
    return Utils::Vec3(
        Utils::randomDouble() - 0.5, Utils::randomDouble() - 0.5, 0);
}

/**
 * @brief Sample a disk.
 *
 * This function returns a random point in a disk centered at the origin with
 * a radius of 1.
 *
 * @param radius The radius of the disk.
 * @return A random point in a disk.
 */
Raytracer::Utils::Vec3 Raytracer::Core::Camera::sampleDisk(double radius) const
{
    return radius * Utils::randomInUnitDisk<double, 3>();
}

/**
 * @brief Sample the defocus disk.
 *
 * This function returns a random point in the defocus disk.
 *
 * @return A random point in the defocus disk.
 */
Raytracer::Utils::Vec3 Raytracer::Core::Camera::sampleDefocusDisk() const
{
    Utils::Vec3 p = Utils::randomInUnitDisk<double, 3>();

    return _center + (p.x() * _defocusDiskU) + (p.y() * _defocusDiskV);
}

/**
 * @brief Get the color of the ray.
 *
 * This function returns the color of the ray based on the depth and the world.
 * If the depth is less than or equal to 0, the function returns black. If the
 * ray does not hit anything in the world, the function returns the background
 * color. If the ray scatters, the function returns the emission color plus the
 * scatter color.
 *
 * @param ray The ray to get the color of.
 * @param depth The depth of the ray.
 * @param world The world to get the color from.
 * @return The color of the ray.
 */
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

/**
 * @brief Print the progress of the rendering.
 *
 * This function prints the progress of the rendering to the standard error
 * stream. The progress is printed as a percentage and the time elapsed is
 * printed in seconds.
 *
 * @param start The start time of the rendering.
 * @param j The y coordinate of the pixel.
 * @return void
 */
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
