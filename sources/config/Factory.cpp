#include "config/Factory.hpp"
#include <memory>
#include "arguments/Effects.hpp"
#include "arguments/Materials.hpp"
#include "arguments/Shapes.hpp"
#include "arguments/Textures.hpp"
#include "effects/RotateX.hpp"
#include "effects/RotateY.hpp"
#include "effects/RotateZ.hpp"
#include "effects/Smoke.hpp"
#include "effects/Translate.hpp"
#include "interfaces/IArguments.hpp"
#include "materials/Dielectric.hpp"
#include "materials/DiffuseLight.hpp"
#include "materials/Isotropic.hpp"
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

Raytracer::Config::FactoryMap<Raytracer::Interfaces::ITexture,
    Raytracer::Config::ConfigTextures>
    Raytracer::Config::Factory::textures = {
        {
            "solid",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Solid> args =
                    std::dynamic_pointer_cast<Arguments::Solid>(raw);
                std::shared_ptr<Raytracer::Textures::SolidColor> texture =
                    std::make_shared<Raytracer::Textures::SolidColor>(
                        args->color());

                return texture;
            },
        },
        {
            "noise",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Noise> args =
                    std::dynamic_pointer_cast<Arguments::Noise>(raw);
                std::shared_ptr<Raytracer::Textures::Noise> texture =
                    std::make_shared<Raytracer::Textures::Noise>(
                        args->scale());

                return texture;
            },
        },
        {
            "image",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Image> args =
                    std::dynamic_pointer_cast<Arguments::Image>(raw);
                std::shared_ptr<Raytracer::Textures::Image> texture =
                    std::make_shared<Raytracer::Textures::Image>(
                        args->filename());

                return texture;
            },
        },
        {
            "checker",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Checker> args =
                    std::dynamic_pointer_cast<Arguments::Checker>(raw);
                std::shared_ptr<Raytracer::Textures::Checker> texture;

                if (args->kind()
                    == Arguments::ArgumentKind::ARG_CHECKER_COLOR) {
                    texture = std::make_shared<Raytracer::Textures::Checker>(
                        args->scale(), args->color1(), args->color2());
                } else {
                    texture = std::make_shared<Raytracer::Textures::Checker>(
                        args->scale(), args->texture1(), args->texture2());
                }

                return texture;
            },
        },
};

Raytracer::Config::FactoryMap<Raytracer::Interfaces::IHittable,
    Raytracer::Config::ConfigEffects>
    Raytracer::Config::Factory::effects = {
        {
            "rotate_x",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::RotateX> args =
                    std::dynamic_pointer_cast<Arguments::RotateX>(raw);
                std::shared_ptr<Raytracer::Effects::RotateX> effect =
                    std::make_shared<Raytracer::Effects::RotateX>(
                        args->object(), args->angle());

                return effect;
            },
        },
        {
            "rotate_y",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::RotateY> args =
                    std::dynamic_pointer_cast<Arguments::RotateY>(raw);
                std::shared_ptr<Raytracer::Effects::RotateY> effect =
                    std::make_shared<Raytracer::Effects::RotateY>(
                        args->object(), args->angle());

                return effect;
            },
        },
        {
            "rotate_z",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::RotateZ> args =
                    std::dynamic_pointer_cast<Arguments::RotateZ>(raw);
                std::shared_ptr<Raytracer::Effects::RotateZ> effect =
                    std::make_shared<Raytracer::Effects::RotateZ>(
                        args->object(), args->angle());

                return effect;
            },
        },
        {
            "translate",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Translate> args =
                    std::dynamic_pointer_cast<Arguments::Translate>(raw);
                std::shared_ptr<Raytracer::Effects::Translate> effect =
                    std::make_shared<Raytracer::Effects::Translate>(
                        args->object(), args->offset());

                return effect;
            },
        },
        {
            "smoke",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Smoke> args =
                    std::dynamic_pointer_cast<Arguments::Smoke>(raw);
                std::shared_ptr<Raytracer::Effects::Smoke> effect;

                if (args->kind() == Arguments::ArgumentKind::ARG_SMOKE_COLOR) {
                    effect = std::make_shared<Raytracer::Effects::Smoke>(
                        args->object(), args->density(), args->color());
                } else {
                    effect = std::make_shared<Raytracer::Effects::Smoke>(
                        args->object(), args->density(), args->texture());
                }

                return effect;
            },
        }};

Raytracer::Config::FactoryMap<Raytracer::Interfaces::IMaterial,
    Raytracer::Config::ConfigMaterials>
    Raytracer::Config::Factory::materials = {
        {
            "lambertian",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Lambertian> args =
                    std::dynamic_pointer_cast<Arguments::Lambertian>(raw);
                std::shared_ptr<Raytracer::Materials::Lambertian> material;

                if (args->kind()
                    == Arguments::ArgumentKind::ARG_LAMBERTIAN_COLOR) {
                    material =
                        std::make_shared<Raytracer::Materials::Lambertian>(
                            args->color());
                } else {
                    material =
                        std::make_shared<Raytracer::Materials::Lambertian>(
                            args->texture());
                }

                return material;
            },
        },
        {
            "dielectric",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Dielectric> args =
                    std::dynamic_pointer_cast<Arguments::Dielectric>(raw);
                std::shared_ptr<Raytracer::Materials::Dielectric> material;

                if (args->kind() == Arguments::ArgumentKind::ARG_DIELECTRIC) {
                    material =
                        std::make_shared<Raytracer::Materials::Dielectric>(
                            args->refractionIndex());
                } else {
                    material =
                        std::make_shared<Raytracer::Materials::Dielectric>(
                            args->refractionIndex(), args->color());
                }

                return material;
            },
        },
        {
            "diffuse_light",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::DiffuseLight> args =
                    std::dynamic_pointer_cast<Arguments::DiffuseLight>(raw);
                std::shared_ptr<Raytracer::Materials::DiffuseLight> material;

                if (args->kind()
                    == Arguments::ArgumentKind::ARG_DIFFUSE_LIGHT_COLOR) {
                    material =
                        std::make_shared<Raytracer::Materials::DiffuseLight>(
                            args->color());
                } else {
                    material =
                        std::make_shared<Raytracer::Materials::DiffuseLight>(
                            args->texture());
                }

                return material;
            },
        },
        {
            "isotropic",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Isotropic> args =
                    std::dynamic_pointer_cast<Arguments::Isotropic>(raw);
                std::shared_ptr<Raytracer::Materials::Isotropic> material;

                if (args->kind()
                    == Arguments::ArgumentKind::ARG_ISOTROPIC_COLOR) {
                    material =
                        std::make_shared<Raytracer::Materials::Isotropic>(
                            args->color());
                } else {
                    // FIXME: No matching constructor for this call
                    material =
                        std::make_shared<Raytracer::Materials::Isotropic>(
                            args->texture());
                }

                return material;
            },
        },
        {
            "metal",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Metal> args =
                    std::dynamic_pointer_cast<Arguments::Metal>(raw);
                std::shared_ptr<Raytracer::Materials::Metal> material =
                    std::make_shared<Raytracer::Materials::Metal>(
                        args->color(), args->fuzz());

                return material;
            },
        },
};

Raytracer::Config::FactoryMap<Raytracer::Interfaces::IHittable,
    Raytracer::Config::ConfigShapes>
    Raytracer::Config::Factory::shapes = {
        {
            "cone",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Cone> args =
                    std::dynamic_pointer_cast<Arguments::Cone>(raw);
                std::shared_ptr<Raytracer::Shapes::Cone> shape =
                    std::make_shared<Raytracer::Shapes::Cone>(args->center(),
                        args->radius(), args->height(), args->material());

                return shape;
            },
        },
        {
            "cylinder",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Cylinder> args =
                    std::dynamic_pointer_cast<Arguments::Cylinder>(raw);
                std::shared_ptr<Raytracer::Shapes::Cylinder> shape =
                    std::make_shared<Raytracer::Shapes::Cylinder>(
                        args->center(), args->radius(), args->height(),
                        args->material());

                return shape;
            },
        },
        {
            "plane",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Plane> args =
                    std::dynamic_pointer_cast<Arguments::Plane>(raw);
                std::shared_ptr<Raytracer::Shapes::Plane> shape =
                    std::make_shared<Raytracer::Shapes::Plane>(
                        args->point(), args->normal(), args->material());

                return shape;
            },
        },
        {
            "quad",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Quad> args =
                    std::dynamic_pointer_cast<Arguments::Quad>(raw);
                std::shared_ptr<Raytracer::Shapes::Quad> shape =
                    std::make_shared<Raytracer::Shapes::Quad>(
                        args->Q(), args->u(), args->v(), args->material());

                return shape;
            },
        },
        {
            "sphere",
            [](std::shared_ptr<Interfaces::IArguments> raw) {
                std::shared_ptr<Arguments::Sphere> args =
                    std::dynamic_pointer_cast<Arguments::Sphere>(raw);
                std::shared_ptr<Raytracer::Shapes::Sphere> shape;

                if (args->kind()
                    == Arguments::ArgumentKind::ARG_SPHERE_MOVING) {
                    shape = std::make_shared<Raytracer::Shapes::Sphere>(
                        args->center(), args->centerTwo(), args->radius(),
                        args->material());
                } else {
                    // FIXME: No matching constructor for this call
                    shape = std::make_shared<Raytracer::Shapes::Sphere>(
                        args->center(), args->radius(), args->material());
                }

                return shape;
            },
        },
};
