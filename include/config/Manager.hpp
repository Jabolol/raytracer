#include <functional>
#include <libconfig.hh>
#include <memory>
#include <vector>
#include "Common.hpp"
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
        std::vector<std::shared_ptr<Interfaces::ITexture>> _textures;
        std::vector<std::shared_ptr<Interfaces::IHittable>> _effects;
        std::vector<std::shared_ptr<Interfaces::IMaterial>> _materials;
        std::vector<std::shared_ptr<Interfaces::IHittable>> _shapes;

        std::unordered_map<std::string, std::shared_ptr<Interfaces::ITexture>>
            _texturesMap;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::IHittable>>
            _effectsMap;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::IMaterial>>
            _materialsMap;
        std::unordered_map<std::string, std::shared_ptr<Interfaces::IHittable>>
            _shapesMap;

      public:
        bool parse(std::string path);
        GET_SET(std::vector<std::shared_ptr<Interfaces::ITexture>>, textures);
        GET_SET(std::vector<std::shared_ptr<Interfaces::IHittable>>, effects);
        GET_SET(
            std::vector<std::shared_ptr<Interfaces::IMaterial>>, materials);
        GET_SET(std::vector<std::shared_ptr<Interfaces::IHittable>>, shapes);
        static std::unordered_map<std::string,
            std::function<std::shared_ptr<Interfaces::IArguments>(
                libconfig::Setting &)>>
            argumentMap;
        static Utils::Color parseColor(const libconfig::Setting &color);

      private:
        void parseTextures(const libconfig::Setting &textures);
        void parseEffects(const libconfig::Setting &effects);
        void parseMaterials(const libconfig::Setting &materials);
        void parseShapes(const libconfig::Setting &shapes);
        std::shared_ptr<Raytracer::Interfaces::IArguments> create(
            const std::string &type, libconfig::Setting &args);
    };
} // namespace Raytracer::Config

#endif /* __CFG_MANAGER_HPP__ */
