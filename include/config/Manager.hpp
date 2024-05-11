#include <functional>
#include <libconfig.hh>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include "Common.hpp"
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "interfaces/IArguments.hpp"
#include "interfaces/IHittable.hpp"
#include "interfaces/IMaterial.hpp"
#include "interfaces/ITexture.hpp"
#include "libconfig.h++"
#include <type_traits>
#include <unordered_map>

#ifndef __CFG_MANAGER_HPP__
    #define __CFG_MANAGER_HPP__

namespace Raytracer::Config
{
    template <typename I>
    using ManagerMap = std::unordered_map<std::string, std::shared_ptr<I>>;

    using KeyTypes = std::tuple<double, int, int, int, Raytracer::Utils::Color,
        int, Raytracer::Utils::Point3, Raytracer::Utils::Point3,
        Raytracer::Utils::Point3, double>;

    template <int I> using KeyType = std::tuple_element_t<I, KeyTypes>;

    using CameraTypes = std::variant<int, double, Raytracer::Utils::Vec3>;

    class Manager {
      private:
        Raytracer::Core::Scene _world;
        Raytracer::Core::Camera _camera;
        std::vector<std::string> _ids;
        ManagerMap<Interfaces::ITexture> _textures;
        ManagerMap<Interfaces::IHittable> _effects;
        ManagerMap<Interfaces::IMaterial> _materials;
        ManagerMap<Interfaces::IHittable> _shapes;
        std::unordered_map<std::string,
            std::function<std::shared_ptr<Interfaces::IArguments>(
                libconfig::Setting &)>>
            _argumentMap;
        std::unordered_map<std::string,
            std::function<void(Raytracer::Core::Camera &, CameraTypes &)>>
            _cameraMap;

      public:
        Manager();
        bool parse(std::string path);
        void bootstrap();
        void render(bool fast);
        GET_SET(Raytracer::Core::Scene, world);
        GET_SET(Raytracer::Core::Camera, camera);

      private:
        template <typename I, typename E>
            requires std::is_enum_v<E>
        void genericParse(
            const libconfig::Setting &arguments, ManagerMap<I> &containerMap);
        static Utils::Color parseColor(const libconfig::Setting &color);
        template <typename I>
        std::shared_ptr<I> retrieve(const libconfig::Setting &arguments,
            ManagerMap<I> &containerMap, const std::string &name);
        std::shared_ptr<Raytracer::Interfaces::IArguments> create(
            const std::string &type, libconfig::Setting &args);
        void parseCamera(const libconfig::Setting &camera);
        void parseImports(const libconfig::Setting &imports);
        template <typename T>
            requires std::is_arithmetic_v<T>
        std::optional<T> parseOptional(
            const libconfig::Setting &setting, std::string &name);
        template <typename T>
            requires std::is_same_v<T, Raytracer::Utils::Vec3>
        std::optional<T> parseOptional(
            const libconfig::Setting &setting, std::string &name);
        template <std::size_t I>
        void extract(const libconfig::Setting &setting,
            std::array<std::string, 10> &keys);
        template <std::size_t... Is>
        void parseCameraHelper(const libconfig::Setting &camera,
            std::array<std::string, 10> &keys, std::index_sequence<Is...>);
    };
} // namespace Raytracer::Config

#endif /* __CFG_MANAGER_HPP__ */
