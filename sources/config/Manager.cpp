#include "config/Manager.hpp"
#include <array>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <libconfig.hh>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include "arguments/Effects.hpp"
#include "arguments/Materials.hpp"
#include "arguments/Shapes.hpp"
#include "arguments/Textures.hpp"
#include "config/Factory.hpp"
#include "exceptions/Argument.hpp"
#include "exceptions/Cyclic.hpp"
#include "exceptions/File.hpp"
#include "exceptions/Missing.hpp"
#include "exceptions/Parse.hpp"
#include "interfaces/IArguments.hpp"
#include "utils/VecN.hpp"
#include "libconfig.h++"
#include <type_traits>
#include <unordered_map>

/**
 * @brief Construct a new Manager:: Manager object
 *
 * Initialize the argument map with the available arguments and their
 * corresponding functions. Initialize the camera map with the available camera
 * arguments and their corresponding functions.
 */
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
                    throw Exceptions::FileException(
                        std::format("file `{}` not found", path));
                }

                if (p.extension() != ".ppm") {
                    throw Exceptions::FileException(
                        std::format("file `{}` is not a PPM file", path));
                }

                if (std::filesystem::file_size(p) == 0) {
                    throw Exceptions::FileException(
                        std::format("file `{}` is empty", path));
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

                throw Exceptions::ArgumentException(
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

                throw Exceptions::ArgumentException(
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

                throw Exceptions::ArgumentException(
                    "lambertian material must have color or texture");
            },
        },
        {
            "dielectric",
            [](libconfig::Setting &args) {
                double refraction = args["refraction_index"];

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

                throw Exceptions::ArgumentException(
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

                throw Exceptions::ArgumentException(
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

                throw Exceptions::ArgumentException(
                    "sphere shape must have center or "
                    "center_one and center_two");
            },
        },
    };
    _cameraMap = {
        {
            "aspect_ratio",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.aspectRatio(std::get<double>(value));
            },
        },
        {
            "image_width",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.imageWidth(std::get<int>(value));
            },
        },
        {
            "samples_per_pixel",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.samplesPerPixel(std::get<int>(value));
            },
        },
        {
            "max_depth",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.maxDepth(std::get<int>(value));
            },
        },
        {
            "background_color",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.backgroundColor(
                    std::get<Raytracer::Utils::Color>(value));
            },
        },
        {
            "v_fov",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.vFov(std::get<int>(value));
            },
        },
        {
            "look_from",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.lookFrom(std::get<Raytracer::Utils::Vec3>(value));
            },
        },
        {
            "look_at",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.lookAt(std::get<Raytracer::Utils::Vec3>(value));
            },
        },
        {
            "v_up",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.vUp(std::get<Raytracer::Utils::Vec3>(value));
            },
        },
        {
            "defocus_angle",
            [](Raytracer::Core::Camera &camera, CameraTypes &value) {
                camera.defocusAngle(std::get<double>(value));
            },
        },
    };
}

/**
 * @brief Parse the configuration file
 *
 * Parse the configuration file and load the scene, textures, materials, shapes
 * and effects. If an error occurs while parsing the file, an exception is
 * thrown.
 *
 * @param path Path to the configuration file
 * @throw Exceptions::ParseException if an error occurs while parsing the file
 * @throw Exceptions::CyclicException if a cyclic dependency is detected
 * @throw Exceptions::MissingException if a path is not found
 * @throw Exceptions::ParseException if a path is not of the correct type
 * @throw Exceptions::MissingException if an argument is not found
 *
 * @return void
 */
void Raytracer::Config::Manager::parse(std::string path)
{
    libconfig::Config config;

    try {
        config.readFile(path.c_str());
    } catch (const libconfig::FileIOException &e) {
        std::cerr << "I/O error while reading file." << std::endl;
        throw Exceptions::ParseException("I/O error while reading file.");
    } catch (const libconfig::ParseException &e) {
        throw Exceptions::ParseException(
            std::format("Parse exception at {}:{} - {}", e.getFile(),
                e.getLine(), e.getError()));
    }

    const libconfig::Setting &root = config.getRoot();
    try {
        const libconfig::Setting &scene = root["scene"];
        std::string id = scene["id"];

        if (std::find(_ids.begin(), _ids.end(), id) != _ids.end()) {
            throw Exceptions::CyclicException(std::format(
                "cyclic dependency detected (id `{}` already loaded)", id));
        }

        _ids.push_back(id);
        parseImports(scene["imports"]);
        parseCamera(scene["camera"]);
        genericParse<Interfaces::ITexture, ConfigTextures>(
            scene["textures"], _textures);
        genericParse<Interfaces::IMaterial, ConfigMaterials>(
            scene["materials"], _materials);
        genericParse<Interfaces::IHittable, ConfigShapes>(
            scene["shapes"], _shapes);
        genericParse<Interfaces::IHittable, ConfigEffects>(
            scene["effects"], _effects);
    } catch (const libconfig::SettingNotFoundException &e) {
        throw Exceptions::ParseException(
            std::format("path `{}` not found, aborting...", e.getPath()));
    } catch (const libconfig::SettingTypeException &e) {
        throw Exceptions::ParseException(
            std::format("{} is not of the correct type", e.getPath()));
    }
}

/**
 * @brief Generic parse function
 *
 * Parse the configuration file, create the objects and store them in the
 * container map. If an error occurs while parsing the file, an exception is
 * thrown.
 *
 * @tparam I Interface that the object must implement
 * @tparam E Enum that the object must be associated with
 * @param setting Setting to parse
 * @param containerMap Map to store the parsed objects
 * @throw Exceptions::CyclicException if a cyclic dependency is detected
 *
 * @return void
 */
template <typename I, typename E>
    requires std::is_enum_v<E>
void Raytracer::Config::Manager::genericParse(
    const libconfig::Setting &setting, ManagerMap<I> &containerMap)
{
    for (int i = 0; i < setting.getLength(); i++) {
        const libconfig::Setting &root = setting[i];
        std::string id = root["id"];

        if (containerMap.contains(id)) {
            throw Exceptions::CyclicException(std::format(
                "cyclic dependency detected (id `{}` already loaded)", id));
        }

        std::string type = root["type"];
        libconfig::Setting &args = root["args"];
        std::shared_ptr<Interfaces::IArguments> argument = create(type, args);

        if (argument == nullptr) {
            throw Exceptions::MissingException("failed to create argument");
        }

        std::shared_ptr<I> resource = Factory::get<I, E>(type, argument);

        if (std::string(setting.getName()) == "effects") {
            containerMap[root["args"]["target"]] = resource;
            continue;
        }

        containerMap[id] = resource;
    }
}

/**
 * @brief Create an argument object
 *
 * Create an argument object based on the type and arguments provided. If the
 * type is not found in the argument map, return a nullptr.
 *
 * @param type Type of the argument
 * @param args Arguments to pass to the argument object
 * @throw libconfig::SettingTypeException if the type is not found in the
 * argument map
 *
 * @return std::shared_ptr<Raytracer::Interfaces::IArguments> Argument object
 */
std::shared_ptr<Raytracer::Interfaces::IArguments>
Raytracer::Config::Manager::create(
    const std::string &type, libconfig::Setting &args)
{
    if (!_argumentMap.contains(type)) {
        return nullptr;
    }

    try {
        return _argumentMap[type](args);
    } catch (libconfig::SettingTypeException &e) {
        return nullptr;
    }
}

/**
 * @brief Parse a color
 *
 * Parse a color from the configuration file and return it as a Color object.
 *
 * @param color Color to parse
 *
 * @return Raytracer::Utils::Color Parsed color
 */
Raytracer::Utils::Color Raytracer::Config::Manager::parseColor(
    const libconfig::Setting &color)
{
    return Utils::Color(color[0], color[1], color[2]);
}

/**
 * @brief Retrieve an object from the container map
 *
 * Retrieve an object from the container map based on the name provided. If the
 * name is not found in the arguments, an exception is thrown. If the name is
 * not found in the map, an exception is thrown.
 *
 * @tparam I Interface that the object must implement
 * @param arguments Arguments to retrieve the object from
 * @param containerMap Map to retrieve the object from
 * @param name Name of the object to retrieve
 * @throw Exceptions::MissingException if the argument is not found in the
 * arguments
 * @throw Exceptions::MissingException if the argument is not found in the map
 *
 * @return std::shared_ptr<I> Retrieved object
 */
template <typename I>
std::shared_ptr<I> Raytracer::Config::Manager::retrieve(
    const libconfig::Setting &arguments, ManagerMap<I> &containerMap,
    const std::string &name)
{
    if (!arguments.exists(name)) {
        throw Exceptions::MissingException(std::format(
            "argument `{}` not found in {}", name, arguments.getPath()));
    }

    if (!containerMap.contains(arguments[name])) {
        throw Exceptions::MissingException(
            std::format("argument `{}` not found in map", name));
    }

    return containerMap.at(arguments[name]);
}

/**
 * @brief Parse the imports
 *
 * Parse the imports from the configuration file and load the files. If an
 * error occurs while parsing the file, an exception is thrown.
 *
 * @param imports Imports to parse
 * @throw Exceptions::ParseException if an error occurs while parsing the file
 * @throw Exceptions::MissingException if a path is not found
 *
 * @return void
 */
void Raytracer::Config::Manager::parseImports(
    const libconfig::Setting &imports)
{
    for (int i = 0; i < imports.getLength(); i++) {
        libconfig::Setting &root = imports[i];
        parse(root["path"]);
    }
}

/**
 * @brief Extract the camera arguments
 *
 * Extract the camera arguments from the configuration file and set them in the
 * camera object.
 *
 * @tparam I Index of the argument
 * @param setting Setting to extract the arguments from
 * @param keys Keys to extract the arguments from
 *
 * @return void
 */
template <std::size_t I>
void Raytracer::Config::Manager::extract(
    const libconfig::Setting &setting, std::array<std::string, 10> &keys)
{
    if constexpr (I != 0) {
        constexpr std::size_t F = I - 1;
        using T = Raytracer::Config::KeyType<F>;
        std::optional<T> value = parseOptional<T>(setting, keys[F]);

        if (value.has_value()) {
            CameraTypes arg = value.value();
            _cameraMap[keys[F]](_camera, arg);
        }

        extract<F>(setting, keys);
    }
}

/**
 * @brief Parse the camera arguments
 *
 * Parse the camera arguments from the configuration file and set them in the
 * camera object.
 *
 * @param camera Camera arguments to parse
 * @throw Exceptions::MissingException if an argument is not found
 *
 * @return void
 */
void Raytracer::Config::Manager::parseCamera(const libconfig::Setting &camera)
{
    std::array<std::string, 10> keys = {
        "aspect_ratio",
        "image_width",
        "samples_per_pixel",
        "max_depth",
        "background_color",
        "v_fov",
        "look_from",
        "look_at",
        "v_up",
        "defocus_angle",
    };

    try {
        extract<10>(camera, keys);
    } catch (const std::bad_variant_access &e) {
        throw Exceptions::MissingException(
            "invalid variant access for camera argument");
    }
}

/**
 * @brief Parse an optional argument
 *
 * Parse an optional argument from the configuration file and return it as an
 * optional object.
 *
 * @tparam T Type of the argument
 * @param setting Setting to parse
 * @param name Name of the argument
 *
 * @return std::optional<T> Parsed optional argument
 */
template <typename T>
    requires std::is_arithmetic_v<T>
std::optional<T> Raytracer::Config::Manager::parseOptional(
    const libconfig::Setting &setting, std::string &name)
{
    if (!setting.exists(name)) {
        return std::nullopt;
    }

    return setting[name];
}

/**
 * @brief Parse an optional argument
 *
 * Parse an optional argument from the configuration file and return it as an
 * optional object.
 *
 * @tparam T Type of the argument
 * @param setting Setting to parse
 * @param name Name of the argument
 *
 * @return std::optional<T> Parsed optional argument
 */
template <typename T>
    requires std::is_same_v<T, Raytracer::Utils::Vec3>
std::optional<T> Raytracer::Config::Manager::parseOptional(
    const libconfig::Setting &setting, std::string &name)
{
    if (!setting.exists(name)) {
        return std::nullopt;
    }

    return Raytracer::Config::Manager::parseColor(setting[name]);
}

/**
 * @brief Bootstrap the configuration
 *
 * Bootstrap the configuration by adding the shapes to the world.
 *
 * @return void
 */
void Raytracer::Config::Manager::bootstrap()
{
    for (auto &[id, shape] : _shapes) {
        if (_effects.contains(id)) {
            _world.add(_effects.at(id));
        } else {
            _world.add(shape);
        }
    }
}

/**
 * @brief Render the scene
 *
 * Render the scene using the camera and the world. If the fast flag is set,
 * the rendering is done in fast mode, which reduces the image width, the
 * samples per pixel and the maximum depth.
 *
 * @param fast Fast rendering mode
 *
 * @return void
 */
void Raytracer::Config::Manager::render(bool fast)
{
    if (fast) {
        _camera.imageWidth(300);
        _camera.samplesPerPixel(10);
        _camera.maxDepth(50);
    }
    _camera.render(_world);
}
