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
#include "shapes/Cone.hpp"
#include "shapes/Cylinder.hpp"
#include "shapes/Plane.hpp"
#include "shapes/Quad.hpp"
#include "shapes/Sphere.hpp"
#include "textures/Checker.hpp"
#include "textures/Image.hpp"
#include "textures/Noise.hpp"
#include "textures/SolidColor.hpp"
#include "utils/BVHNode.hpp"

int main(int argc, char **argv)
{
    Raytracer::Core::Scene world;

    auto red = std::make_shared<Raytracer::Materials::Lambertian>(
        Raytracer::Utils::Color(.65, .05, .05));
    auto white = std::make_shared<Raytracer::Materials::Lambertian>(
        Raytracer::Utils::Color(.73, .73, .73));
    auto green = std::make_shared<Raytracer::Materials::Lambertian>(
        Raytracer::Utils::Color(.12, .45, .15));
    auto light = std::make_shared<Raytracer::Materials::DiffuseLight>(
        Raytracer::Utils::Color(15, 15, 15));
    auto metal = std::make_shared<Raytracer::Materials::Metal>(
        Raytracer::Utils::Color(.8, .8, .9), 0.0);
    auto glass = std::make_shared<Raytracer::Materials::Dielectric>(1.5);

    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(555, 0, 0), Raytracer::Utils::Vec3(0, 555, 0),
        Raytracer::Utils::Vec3(0, 0, 555), green));
    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(0, 0, 0), Raytracer::Utils::Vec3(0, 555, 0),
        Raytracer::Utils::Vec3(0, 0, 555), red));
    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(343, 554, 332),
        Raytracer::Utils::Vec3(-130, 0, 0), Raytracer::Utils::Vec3(0, 0, -105),
        light));
    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(0, 0, 0), Raytracer::Utils::Vec3(555, 0, 0),
        Raytracer::Utils::Vec3(0, 0, 555), white));
    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(555, 555, 555),
        Raytracer::Utils::Vec3(-555, 0, 0), Raytracer::Utils::Vec3(0, 0, -555),
        white));
    world.add(std::make_shared<Raytracer::Shapes::Quad>(
        Raytracer::Utils::Point3(0, 0, 555), Raytracer::Utils::Vec3(555, 0, 0),
        Raytracer::Utils::Vec3(0, 555, 0), white));

    std::shared_ptr<Raytracer::Interfaces::IHittable> box1 =
        Raytracer::Shapes::box(Raytracer::Utils::Point3(0, 0, 0),
            Raytracer::Utils::Point3(165, 330, 165), metal);
    box1 = std::make_shared<Raytracer::Effects::Rotate>(box1, 15);
    box1 = std::make_shared<Raytracer::Effects::Translate>(
        box1, Raytracer::Utils::Vec3(265, 0, 295));
    world.add(box1);

    world.add(std::make_shared<Raytracer::Shapes::Sphere>(
        Raytracer::Utils::Point3(190, 90, 190), 90, glass));

    Raytracer::Core::Camera camera;

    camera.aspectRatio(1.0);
    camera.imageWidth(600);
    camera.samplesPerPixel(50);
    camera.maxDepth(50);
    camera.backgroundColor(Raytracer::Utils::Color(0, 0, 0));

    camera.vFov(40);
    camera.lookFrom(Raytracer::Utils::Point3(278, 278, -800));
    camera.lookAt(Raytracer::Utils::Point3(278, 278, 0));
    camera.vUp(Raytracer::Utils::Vec3(0, 1, 0));

    camera.defocusAngle(0);

    if (argc > 1) {
        char **fast = std::find_if(argv, argv + argc, [](const char *arg) {
            return std::string(arg) == "--fast";
        });

        if (fast != argv + argc) {
            camera.samplesPerPixel(20);
            camera.maxDepth(20);
            camera.imageWidth(300);
        }
    }

    camera.render(world);
}
