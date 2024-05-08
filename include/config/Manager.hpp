#include <functional>
#include <libconfig.hh>
#include <memory>
#include <string>
#include "core/Camera.hpp"
#include "core/Scene.hpp"
#include "interfaces/IArguments.hpp"
#include "interfaces/IHittable.hpp"
#include "interfaces/IMaterial.hpp"
#include "interfaces/ITexture.hpp"
#include "libconfig.h++"
#include <unordered_map>

#ifndef __CFG_MANAGER_HPP__
    #define __CFG_MANAGER_HPP__

namespace Raytracer::Config
{
    class Manager {
      private:
        libconfig::Config _config;
        Raytracer::Core::Scene _world;
        Raytracer::Core::Camera _camera;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::ITexture>>
            _textures;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::IHittable>>
            _effects;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::IMaterial>>
            _materials;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::IHittable>>
            _shapes;

      public:
        Manager();
        bool parse(std::string path);
        std::unordered_map<std::string,
            std::function<std::shared_ptr<Interfaces::IArguments>(
                libconfig::Setting &)>>
            _argumentMap;
        static Utils::Color parseColor(const libconfig::Setting &color);

      private:
        template <typename I, typename E>
            requires std::is_enum_v<E>
        void genericParse(const libconfig::Setting &arguments,
            std::unordered_map<std::string, std::shared_ptr<I>> &containerMap);
        template <typename I>
        std::shared_ptr<I> retrieve(const libconfig::Setting &arguments,
            std::unordered_map<std::string, std::shared_ptr<I>> &containerMap,
            const std::string &name);
        std::shared_ptr<Raytracer::Interfaces::IArguments> create(
            const std::string &type, libconfig::Setting &args);
    };
} // namespace Raytracer::Config

#endif /* __CFG_MANAGER_HPP__ */
