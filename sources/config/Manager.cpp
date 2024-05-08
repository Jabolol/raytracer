#include "config/Manager.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <libconfig.hh>
#include <memory>
#include <stdexcept>
#include <string>
#include "arguments/Effects.hpp"
#include "arguments/Materials.hpp"
#include "arguments/Shapes.hpp"
#include "arguments/Textures.hpp"
#include "config/Factory.hpp"
#include "interfaces/IArguments.hpp"
#include "utils/VecN.hpp"
#include "libconfig.h++"
#include <type_traits>
#include <unordered_map>

Raytracer::Config::Manager::Manager()
{
    _argumentMap = {
        {
            "solid",
            [](libconfig::Setting &args) {
                libconfig::Setting &rgb = args["color"];
                Utils::Color color = Manager::parseColor(rgb);
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Solid>(color);

                return argument;
            },
        },
        {
            "noise",
            [](libconfig::Setting &args) {
                double scale = args["scale"];
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Noise>(scale);

                return argument;
            },
        },
        {
            "image",
            [](libconfig::Setting &args) {
                std::string path = args["path"];
                std::filesystem::path p(path);

                if (!std::filesystem::exists(p)) {
                    std::cerr << "error: file `" << path << "` not found"
                              << std::endl;
                    throw std::runtime_error("file not found");
                }

                if (p.extension() != ".ppm") {
                    std::cerr << "error: file `" << path
                              << "` is not a PPM file" << std::endl;
                    throw std::runtime_error("file is not a PPM file");
                }

                if (std::filesystem::file_size(p) == 0) {
                    std::cerr << "error: file `" << path << "` is empty"
                              << std::endl;
                    throw std::runtime_error("file is empty");
                }

                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Image>(path);

                return argument;
            },
        },
        {
            "checker",
            [this](libconfig::Setting &args) {
                double scale = args["scale"];

                if (args.exists("color_even") && args.exists("color_odd")) {
                    libconfig::Setting &rgb_even = args["color_even"];
                    Utils::Color color_even = Manager::parseColor(rgb_even);
                    libconfig::Setting &rgb_odd = args["color_odd"];
                    Utils::Color color_odd = Manager::parseColor(rgb_odd);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Checker>(
                            scale, color_even, color_odd);

                    return argument;
                }

                if (args.exists("texture_even")
                    && args.exists("texture_odd")) {
                    std::shared_ptr<Interfaces::ITexture> first =
                        retrieve<Interfaces::ITexture>(
                            args, _textures, "texture_even");
                    std::shared_ptr<Interfaces::ITexture> second =
                        retrieve<Interfaces::ITexture>(
                            args, _textures, "texture_odd");

                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Checker>(
                            scale, first, second);

                    return argument;
                }

                throw std::runtime_error(
                    "checker texture must have color_even and color_odd or "
                    "texture_even and texture_odd");
            },
        },
        {
            "rotate_x",
            [this](libconfig::Setting &args) {
                double angle = args["angle"];
                std::shared_ptr<Interfaces::IHittable> hittable =
                    retrieve<Interfaces::IHittable>(args, _shapes, "target");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::RotateX>(hittable, angle);

                return argument;
            },
        },
        {
            "rotate_y",
            [this](libconfig::Setting &args) {
                double angle = args["angle"];
                std::shared_ptr<Interfaces::IHittable> hittable =
                    retrieve<Interfaces::IHittable>(args, _shapes, "target");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::RotateY>(hittable, angle);

                return argument;
            },
        },
        {
            "rotate_z",
            [this](libconfig::Setting &args) {
                double angle = args["angle"];
                std::shared_ptr<Interfaces::IHittable> hittable =
                    retrieve<Interfaces::IHittable>(args, _shapes, "target");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::RotateZ>(hittable, angle);

                return argument;
            },
        },
        {
            "smoke",
            [this](libconfig::Setting &args) {
                double density = args["density"];
                std::shared_ptr<Interfaces::IHittable> hittable =
                    retrieve<Interfaces::IHittable>(args, _shapes, "target");

                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Smoke>(
                            hittable, density, color);

                    return argument;
                }

                if (args.exists("texture")) {
                    std::shared_ptr<Interfaces::ITexture> texture =
                        retrieve<Interfaces::ITexture>(
                            args, _textures, "texture");
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Smoke>(
                            hittable, density, texture);

                    return argument;
                }

                throw std::runtime_error(
                    "smoke effect must have color or texture");
            },
        },
        {
            "translate",
            [this](libconfig::Setting &args) {
                Utils::Vec3 offset = parseColor(args["offset"]);
                std::shared_ptr<Interfaces::IHittable> hittable =
                    retrieve<Interfaces::IHittable>(args, _shapes, "target");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Translate>(hittable, offset);
                _shapes[args["target"]] = hittable;

                return argument;
            },
        },
        {
            "lambertian",
            [this](libconfig::Setting &args) {
                if (args.exists("texture")) {
                    std::shared_ptr<Interfaces::ITexture> texture =
                        retrieve<Interfaces::ITexture>(
                            args, _textures, "texture");
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Lambertian>(texture);

                    return argument;
                }

                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Lambertian>(color);

                    return argument;
                }

                throw std::runtime_error(
                    "lambertian material must have color or texture");
            },
        },
        {
            "dielectric",
            [](libconfig::Setting &args) {
                double refraction = args["refractionIndex"];

                if (args.exists(("color"))) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Dielectric>(
                            refraction, color);
                }

                return std::make_shared<Arguments::Dielectric>(refraction);
            },
        },
        {
            "diffuse_light",
            [this](libconfig::Setting &args) {
                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::DiffuseLight>(color);

                    return argument;
                }

                if (args.exists("texture")) {
                    std::shared_ptr<Interfaces::ITexture> texture =
                        retrieve<Interfaces::ITexture>(
                            args, _textures, "texture");
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::DiffuseLight>(texture);

                    return argument;
                }

                throw std::runtime_error(
                    "diffuse_light material must have color or texture");
            },
        },
        {
            "isotropic",
            [this](libconfig::Setting &args) {
                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Isotropic>(color);

                    return argument;
                }

                if (args.exists("texture")) {
                    std::shared_ptr<Interfaces::ITexture> texture =
                        retrieve<Interfaces::ITexture>(
                            args, _textures, "texture");
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Isotropic>(texture);

                    return argument;
                }

                throw std::runtime_error(
                    "isotropic material must have color or texture");
            },
        },
        {
            "metal",
            [](libconfig::Setting &args) {
                double fuzz = args["fuzz"];
                libconfig::Setting &rgb = args["color"];
                Utils::Color color = Manager::parseColor(rgb);
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Metal>(color, fuzz);

                return argument;
            },
        },
        {
            "cone",
            [this](libconfig::Setting &args) {
                Utils::Point3 center = parseColor(args["center"]);
                double radius = args["radius"];
                double height = args["height"];
                std::shared_ptr<Interfaces::IMaterial> material =
                    retrieve<Interfaces::IMaterial>(
                        args, _materials, "material");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Cone>(
                        center, radius, height, material);

                return argument;
            },
        },
        {
            "cylinder",
            [this](libconfig::Setting &args) {
                Utils::Point3 center = parseColor(args["center"]);
                double radius = args["radius"];
                double height = args["height"];
                std::shared_ptr<Interfaces::IMaterial> material =
                    retrieve<Interfaces::IMaterial>(
                        args, _materials, "material");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Cylinder>(
                        center, radius, height, material);

                return argument;
            },
        },
        {
            "plane",
            [this](libconfig::Setting &args) {
                Utils::Point3 point = parseColor(args["point"]);
                Utils::Vec3 normal = parseColor(args["normal"]);
                std::shared_ptr<Interfaces::IMaterial> material =
                    retrieve<Interfaces::IMaterial>(
                        args, _materials, "material");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Plane>(
                        point, normal, material);

                return argument;
            },
        },
        {
            "quad",
            [this](libconfig::Setting &args) {
                Utils::Point3 q = parseColor(args["q"]);
                Utils::Point3 u = parseColor(args["u"]);
                Utils::Point3 v = parseColor(args["v"]);
                std::shared_ptr<Interfaces::IMaterial> material =
                    retrieve<Interfaces::IMaterial>(
                        args, _materials, "material");
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Quad>(q, u, v, material);

                return argument;
            },
        },
        {
            "sphere",
            [this](libconfig::Setting &args) {
                double radius = args["radius"];
                std::shared_ptr<Interfaces::IMaterial> material =
                    retrieve<Interfaces::IMaterial>(
                        args, _materials, "material");

                if (args.exists("center_one") && args.exists("center_two")) {
                    Utils::Point3 one = parseColor(args["center_one"]);
                    Utils::Point3 two = parseColor(args["center_two"]);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Sphere>(
                            one, two, radius, material);

                    return argument;
                }

                if (args.exists("center")) {
                    Utils::Point3 center = parseColor(args["center"]);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Sphere>(
                            center, radius, material);

                    return argument;
                }

                throw std::runtime_error("sphere shape must have center or "
                                         "center_one and center_two");
            },
        },
    };
}

bool Raytracer::Config::Manager::parse(std::string path)
{
    try {
        _config.readFile(path.c_str());
    } catch (const libconfig::FileIOException &e) {
        std::cerr << "I/O error while reading file." << std::endl;
        return false;
    } catch (const libconfig::ParseException &e) {
        std::cerr << "Parse exception at " << e.getFile() << ":" << e.getLine()
                  << " - " << e.getError() << std::endl;
        return false;
    }

    const libconfig::Setting &root = _config.getRoot();
    try {
        const libconfig::Setting &scene = root["scene"];
        genericParse<Interfaces::ITexture, ConfigTextures>(
            scene["textures"], _textures);
        genericParse<Interfaces::IMaterial, ConfigMaterials>(
            scene["materials"], _materials);
        genericParse<Interfaces::IHittable, ConfigShapes>(
            scene["shapes"], _shapes);
        genericParse<Interfaces::IHittable, ConfigEffects>(
            scene["effects"], _effects);
    } catch (const libconfig::SettingNotFoundException &e) {
        std::cerr << "error: path `" << e.getPath()
                  << "` not found, aborting..." << std::endl;
        return false;
    } catch (const std::runtime_error &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

template <typename I, typename E>
    requires std::is_enum_v<E>
void Raytracer::Config::Manager::genericParse(
    const libconfig::Setting &setting,
    std::unordered_map<std::string, std::shared_ptr<I>> &containerMap)
{
    for (int i = 0; i < setting.getLength(); i++) {
        const libconfig::Setting &root = setting[i];
        std::string type = root["type"];
        libconfig::Setting &args = root["args"];
        std::shared_ptr<Interfaces::IArguments> argument = create(type, args);

        if (argument == nullptr) {
            throw std::runtime_error("failed to create argument");
        }

        std::shared_ptr<I> resource = Factory::get<I, E>(type, argument);

        containerMap[root["id"].c_str()] = resource;
    }
}

std::shared_ptr<Raytracer::Interfaces::IArguments>
Raytracer::Config::Manager::create(
    const std::string &type, libconfig::Setting &args)
{
    if (!_argumentMap.contains(type)) {
        std::cerr << "error: type `" << type << "` not found, aborting..."
                  << std::endl;
        return nullptr;
    }

    try {
        return _argumentMap[type](args);
    } catch (libconfig::SettingTypeException &e) {
        std::cerr << "error: " << e.getPath() << " is not of the correct type"
                  << std::endl;
        return nullptr;
    }
}

Raytracer::Utils::Color Raytracer::Config::Manager::parseColor(
    const libconfig::Setting &color)
{
    return Utils::Color(color[0], color[1], color[2]);
}

template <typename I>
std::shared_ptr<I> Raytracer::Config::Manager::retrieve(
    const libconfig::Setting &arguments,
    std::unordered_map<std::string, std::shared_ptr<I>> &containerMap,
    const std::string &name)
{
    if (!arguments.exists(name)) {
        throw std::runtime_error("error: " + name + " not found");
    }

    if (!containerMap.contains(arguments[name])) {
        throw std::runtime_error("error: " + name + " not found");
    }

    return containerMap.at(arguments[name]);
}
