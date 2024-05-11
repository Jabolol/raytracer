#ifndef __ARG_KINDS_HPP__
#define __ARG_KINDS_HPP__

namespace Raytracer::Arguments
{
    enum class ArgumentKind {
        /* Textures */
        ARG_SOLID_COLOR,
        ARG_SOLID_RGB,
        ARG_NOISE,
        ARG_IMAGE,
        ARG_CHECKER_TEXTURE,
        ARG_CHECKER_COLOR,
        /* Effects */
        ARG_ROTATE_X,
        ARG_ROTATE_Y,
        ARG_ROTATE_Z,
        ARG_SMOKE_TEXTURE,
        ARG_SMOKE_COLOR,
        ARG_TRANSLATE,
        /* Materials */
        ARG_LAMBERTIAN_COLOR,
        ARG_LAMBERTIAN_TEXTURE,
        ARG_DIELECTRIC,
        ARG_DIELECTRIC_COLOR,
        ARG_DIFFUSE_LIGHT_COLOR,
        ARG_DIFFUSE_LIGHT_TEXTURE,
        ARG_ISOTROPIC_COLOR,
        ARG_ISOTROPIC_TEXTURE,
        ARG_METAL,
        /* Shapes */
        ARG_CONE,
        ARG_CYLINDER,
        ARG_PLANE,
        ARG_QUAD,
        ARG_SPHERE,
        ARG_SPHERE_MOVING,
        ARG_BOX,
    };
}

#define ARG_KIND(k)                    \
    ArgumentKind kind() const override \
    {                                  \
        return k;                      \
    }

#endif /* __ARG_KINDS_HPP__ */
