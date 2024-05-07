#include <functional>
#include <memory>
#include <string>
#include "interfaces/IArguments.hpp"
#include "interfaces/IHittable.hpp"
#include "interfaces/IMaterial.hpp"
#include "interfaces/ITexture.hpp"
#include <type_traits>
#include <unordered_map>

#ifndef __CFG_FACTORY_HPP__
    #define __CFG_FACTORY_HPP__

namespace Raytracer::Config
{
    enum class ConfigTextures {
        TEXTURE_SOLID,
        TEXTURE_NOISE,
        TEXTURE_IMAGE,
        TEXTURE_CHECKER,
    };

    enum class ConfigEffects {
        EFFECT_ROTATE_X,
        EFFECT_ROTATE_Y,
        EFFECT_ROTATE_Z,
        EFFECT_SMOKE,
        EFFECT_TRANSLATE,
    };

    enum class ConfigMaterials {
        MATERIAL_LAMBERTIAN,
        MATERIAL_DIELECTRIC,
        MATERIAL_DIFFUSE_LIGHT,
        MATERIAL_ISOTROPIC,
        MATERIAL_METAL,
    };

    enum class ConfigShapes {
        SHAPE_CONE,
        SHAPE_CYLINDER,
        SHAPE_PLANE,
        SHAPE_QUAD,
        SHAPE_SPHERE,
        SHAPE_MOVING_SPHERE,
    };

    template <typename I, typename E>
        requires std::is_enum_v<E>
    using FactoryFunction = std::function<std::shared_ptr<I>(
        std::shared_ptr<Interfaces::IArguments>)>;

    template <typename I, typename E>
    using FactoryMap = std::unordered_map<std::string, FactoryFunction<I, E>>;

    template <typename T, typename E>
    concept isValidEnum = std::is_enum_v<T> && std::is_same_v<T, E>;

    class Factory {
      private:
        Factory() = delete;

        template <typename I, typename E>
        static std::shared_ptr<I> _get(const FactoryMap<I, E> &map,
            const std::string &name,
            std::shared_ptr<Interfaces::IArguments> args)
        {
            return map.at(name)(args);
        }

      public:
        static FactoryMap<Raytracer::Interfaces::ITexture, ConfigTextures>
            textures;
        static FactoryMap<Raytracer::Interfaces::IHittable, ConfigEffects>
            effects;
        static FactoryMap<Raytracer::Interfaces::IMaterial, ConfigMaterials>
            materials;
        static FactoryMap<Raytracer::Interfaces::IHittable, ConfigShapes>
            shapes;

        template <typename I, typename E>
            requires isValidEnum<E, ConfigTextures>
        static std::shared_ptr<I> get(const std::string &name,
            std::shared_ptr<Interfaces::IArguments> args)
        {
            return _get<I, E>(textures, name, args);
        }

        template <typename I, typename E>
            requires isValidEnum<E, ConfigEffects>
        static std::shared_ptr<I> get(const std::string &name,
            std::shared_ptr<Interfaces::IArguments> args)
        {
            return _get<I, E>(effects, name, args);
        }

        template <typename I, typename E>
            requires isValidEnum<E, ConfigMaterials>
        static std::shared_ptr<I> get(const std::string &name,
            std::shared_ptr<Interfaces::IArguments> args)
        {
            return _get<I, E>(materials, name, args);
        }

        template <typename I, typename E>
            requires isValidEnum<E, ConfigShapes>
        static std::shared_ptr<I> get(const std::string &name,
            std::shared_ptr<Interfaces::IArguments> args)
        {
            return _get<I, E>(shapes, name, args);
        }
    };
} // namespace Raytracer::Config

#endif /* __CFG_FACTORY_HPP__ */
