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
#include <unordered_map>

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
        parseTextures(scene["textures"]);
        // ...
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

void Raytracer::Config::Manager::parseTextures(
    const libconfig::Setting &textures)
{
    for (int i = 0; i < textures.getLength(); i++) {
        const libconfig::Setting &texture = textures[i];
        std::string type = texture["type"];
        libconfig::Setting &args = texture["args"];
        std::shared_ptr<Interfaces::IArguments> argument = create(type, args);

        if (argument == nullptr) {
            throw std::runtime_error("failed to create texture argument");
        }

        std::shared_ptr<Interfaces::ITexture> resource =
            Factory::get<Raytracer::Interfaces::ITexture, ConfigTextures>(
                type, argument);

        _textures.push_back(resource);
        _texturesMap[texture["id"].c_str()] = resource;
    }
}

std::shared_ptr<Raytracer::Interfaces::IArguments>
Raytracer::Config::Manager::create(
    const std::string &type, libconfig::Setting &args)
{
    if (argumentMap.find(type) == argumentMap.end()) {
        std::cerr << "error: type `" << type << "` not found, aborting..."
                  << std::endl;
        return nullptr;
    }
    return argumentMap[type](args);
}

Raytracer::Utils::Color Raytracer::Config::Manager::parseColor(
    const libconfig::Setting &color)
{
    return Utils::Color(color[0], color[1], color[2]);
}

std::unordered_map<std::string,
    std::function<std::shared_ptr<Raytracer::Interfaces::IArguments>(
        libconfig::Setting &)>>
    Raytracer::Config::Manager::argumentMap = {
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
            "perlin",
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
            [](libconfig::Setting &args) {
                if (args.exists("color_even") && args.exists("color_odd")) {
                    libconfig::Setting &rgb_even = args["color_even"];
                    Utils::Color color_even = Manager::parseColor(rgb_even);
                    libconfig::Setting &rgb_odd = args["color_odd"];
                    Utils::Color color_odd = Manager::parseColor(rgb_odd);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Checker>(
                            color_even, color_odd);

                    return argument;
                }

                // // TODO: Handle creation of texture pointers
                // if (args.exists("texture_even")
                //     && args.exists("texture_odd")) {
                //     std::string texture_even = args["texture_even"];
                //     std::string texture_odd = args["texture_odd"];
                //     std::shared_ptr<Interfaces::IArguments> argument =
                //         std::make_shared<Arguments::Checker>(
                //             texture_even, texture_odd);

                //     return argument;
                // }

                throw std::runtime_error(
                    "checker texture must have color_even and color_odd or "
                    "texture_even and texture_odd");
            },
        },
        {
            "rotate_x",
            [](libconfig::Setting &args) {
                double angle = args["angle"];
                // TODO: Pass target object as first argument
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::RotateX>(nullptr, angle);

                return argument;
            },
        },
        {
            "rotate_y",
            [](libconfig::Setting &args) {
                double angle = args["angle"];
                // TODO: Pass target object as first argument
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::RotateY>(nullptr, angle);

                return argument;
            },
        },
        {
            "rotate_z",
            [](libconfig::Setting &args) {
                double angle = args["angle"];
                // TODO: Pass target object as first argument
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::RotateZ>(nullptr, angle);

                return argument;
            },
        },
        {
            "smoke",
            [](libconfig::Setting &args) {
                double density = args["density"];
                std::shared_ptr<Interfaces::IArguments> argument;

                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    // TODO: Pass target object as first argument
                    return std::make_shared<Arguments::Smoke>(
                        nullptr, density, color);
                }

                // // TODO: Handle creation of texture pointers
                // if (args.exists("texture")) {
                //     std::string texture = args["texture"];
                //     return std::make_shared<Arguments::Smoke>(
                //         density, texture);
                // }

                throw std::runtime_error(
                    "smoke texture must have color or texture");
            },
        },
        {
            "translate",
            [](libconfig::Setting &args) {
                libconfig::Setting &vec = args["offset"];
                Utils::Vec3 vector(vec[0], vec[1], vec[2]);
                // TODO: Pass target object as first argument
                std::shared_ptr<Interfaces::IArguments> argument =
                    std::make_shared<Arguments::Translate>(nullptr, vector);

                return argument;
            },
        },
        {
            "lambertian",
            [](libconfig::Setting &args) {
                // // TODO: Handle creation of texture pointers
                // if (args.exists("texture")) {
                // std::string texture = args["texture"];
                // std::shared_ptr<Interfaces::IArguments> argument =
                // std::make_shared<Arguments::Lambertian>(texture);
                //
                // return argument;
                // }
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
            [](libconfig::Setting &args) {
                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::DiffuseLight>(color);

                    return argument;
                }
                // // TODO: Handle creation of texture pointers
                // if (args.exists("texture")) {
                //     std::string texture = args["texture"];
                //     std::shared_ptr<Interfaces::IArguments> argument =
                //         std::make_shared<Arguments::DiffuseLight>(texture);

                //     return argument;
                // }

                throw std::runtime_error(
                    "diffuse_light material must have color or texture");
            },
        },
        {
            "isotropic",
            [](libconfig::Setting &args) {
                if (args.exists("color")) {
                    libconfig::Setting &rgb = args["color"];
                    Utils::Color color = Manager::parseColor(rgb);
                    std::shared_ptr<Interfaces::IArguments> argument =
                        std::make_shared<Arguments::Isotropic>(color);

                    return argument;
                }

                // // TODO: Handle creation of texture pointers
                // if (args.exists("texture")) {
                //     std::string texture = args["texture"];
                //     std::shared_ptr<Interfaces::IArguments> argument =
                //         std::make_shared<Arguments::Isotropic>(texture);

                //     return argument;
                // }

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
        // TODO: Add shapes arguments
};
