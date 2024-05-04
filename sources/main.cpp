#include <memory>
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "effects/Rotate.hpp"
#include "effects/Smoke.hpp"
#include "effects/Translate.hpp"
#include "materials/Dielectric.hpp"
#include "materials/DiffuseLight.hpp"
#include "materials/Lambertian.hpp"
#include "materials/Metal.hpp"
#include "shapes/Quad.hpp"
#include "shapes/Sphere.hpp"
#include "textures/Checker.hpp"
#include "textures/Image.hpp"
#include "textures/Noise.hpp"
#include "textures/SolidColor.hpp"
#include "utils/BVHNode.hpp"

int main()
{
    Raytracer::Core::Scene boxes1;
    auto ground = std::make_shared<Raytracer::Materials::Lambertian>(
        Raytracer::Utils::Color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = Raytracer::Utils::randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(
                Raytracer::Shapes::box(Raytracer::Utils::Point3(x0, y0, z0),
                    Raytracer::Utils::Point3(x1, y1, z1), ground));
        }
    }

    Raytracer::Core::Scene world;

    world.add(std::make_shared<Raytracer::Utils::BVHNode>(boxes1));

    auto light = std::make_shared<Raytracer::Materials::DiffuseLight>(
        Raytracer::Utils::Color(7, 7, 7));
    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(123, 554, 147),
        Raytracer::Utils::Vec3(300, 0, 0), Raytracer::Utils::Vec3(0, 0, 265),
        light));

    auto center1 = Raytracer::Utils::Point3(400, 400, 200);
    auto center2 = center1 + Raytracer::Utils::Vec3(30, 0, 0);
    auto sphere_material = std::make_shared<Raytracer::Materials::Lambertian>(
        Raytracer::Utils::Color(0.7, 0.3, 0.1));
    world.add(std::make_shared<Raytracer::Shapes::Sphere>(
        center1, center2, 50, sphere_material));

    world.add(std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(260, 150, 45), 50,
        std::make_shared<Raytracer::Materials::Dielectric>(1.5)));
    world.add(std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(0, 150, 145), 50,
        std::make_shared<Raytracer::Materials::Metal>(
            Raytracer::Utils::Color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(360, 150, 145), 70,
        std::make_shared<Raytracer::Materials::Dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<Raytracer::Effects::Smoke>(
        boundary, 0.2, Raytracer::Utils::Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(0, 0, 0), 5000,
        std::make_shared<Raytracer::Materials::Dielectric>(1.5));
    world.add(std::make_shared<Raytracer::Effects::Smoke>(
        boundary, .0001, Raytracer::Utils::Color(1, 1, 1)));

    auto emat = std::make_shared<Raytracer::Materials::Lambertian>(
        std::make_shared<Raytracer::Textures::Image>(
            "../assets/earthmap.ppm"));
    world.add(std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<Raytracer::Textures::Noise>(0.2);
    world.add(std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(220, 280, 300), 80,
        std::make_shared<Raytracer::Materials::Lambertian>(pertext)));

    Raytracer::Core::Scene boxes2;
    auto white = std::make_shared<Raytracer::Materials::Lambertian>(
        Raytracer::Utils::Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Raytracer::Shapes::Sphere>(
            Raytracer::Utils::Point3::random(0, 165), 10, white));
    }

    world.add(std::make_shared<Raytracer::Effects::Translate>(
        std::make_shared<Raytracer::Effects::Rotate>(
            std::make_shared<Raytracer::Utils::BVHNode>(boxes2), 15),
        Raytracer::Utils::Vec3(-100, 270, 395)));

    Raytracer::Core::Camera camera;

    camera.aspectRatio(1);
    camera.imageWidth(600);
    camera.samplesPerPixel(10);
    camera.maxDepth(50);
    camera.backgroundColor(Raytracer::Utils::Color(0, 0, 0));

    camera.vFov(40);
    camera.lookFrom(Raytracer::Utils::Point3(478, 278, -600));
    camera.lookAt(Raytracer::Utils::Point3(278, 278, 0));
    camera.vUp(Raytracer::Utils::Vec3(0, 1, 0));

    camera.defocusAngle(0);

    camera.render(world);
}
