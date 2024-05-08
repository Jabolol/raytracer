#include <cmath>
#include <cstddef>
#include <iostream>
#include "exceptions/Range.hpp"
#include <type_traits>

#ifndef __VEC_N_HPP__
    #define __VEC_N_HPP__

namespace Raytracer::Utils
{
    inline double degreesToRadians(double degrees)
    {
        return degrees * M_PI / 180.0;
    }

    inline double randomDouble()
    {
        return rand() / (RAND_MAX + 1.0);
    }

    inline double randomDouble(double min, double max)
    {
        return min + (max - min) * randomDouble();
    }

    inline int randomInt(int min, int max)
    {
        return static_cast<int>(randomDouble(min, max + 1));
    }

    template <typename T>
    concept isNumerical = requires(T) { std::is_arithmetic_v<T>; };

    template <typename T>
    concept isPositive = requires(T t) { t > 0; };

    template <isNumerical T, std::size_t N>
        requires isPositive<T> && (N > 1)
    class VecN {
      public:
        T e[N];

        VecN() : e{0, 0, 0}
        {
        }

        VecN(T e0, T e1, T e2)
        {
            static_assert(
                N == 3, "VecN(T e0, T e1, T e2) is only valid for N == 3");
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
        }

        T x() const
        {
            return e[0];
        }

        T y() const
        {
            static_assert(N == 3, "y() is only valid for >= VecN<2>");
            return e[1];
        }

        T z() const
        {
            static_assert(N == 3, "z() is only valid for VecN<3>");
            return e[2];
        }

        VecN operator-() const
        {
            VecN v;
            for (std::size_t i = 0; i < N; i++) {
                v.e[i] = -e[i];
            }

            return v;
        }

        T operator[](std::size_t i) const
        {
            if (i >= N) {
                throw Exceptions::RangeException("Index out of bounds");
            }
            return e[i];
        }

        T &operator[](std::size_t i)
        {
            if (i >= N) {
                throw Exceptions::RangeException("Index out of bounds");
            }
            return e[i];
        }

        VecN &operator+=(const VecN &v)
        {
            for (std::size_t i = 0; i < N; i++) {
                e[i] += v.e[i];
            }

            return *this;
        }

        VecN &operator*=(double t)
        {
            for (std::size_t i = 0; i < N; i++) {
                e[i] *= t;
            }

            return *this;
        }

        VecN &operator/=(double t)
        {
            return *this *= 1 / t;
        }

        double length() const
        {
            return std::sqrt(lengthSquared());
        }

        double lengthSquared() const
        {
            double sum = 0;
            for (std::size_t i = 0; i < N; i++) {
                sum += e[i] * e[i];
            }

            return sum;
        }

        bool nearZero() const
        {
            static constexpr double s = 1e-8;
            for (std::size_t i = 0; i < N; i++) {
                if (std::fabs(e[i]) > s) {
                    return false;
                }
            }

            return true;
        }

        static VecN random()
        {
            VecN result;
            for (std::size_t i = 0; i < N; i++) {
                result[i] = randomDouble();
            }

            return result;
        }

        static VecN random(double min, double max)
        {
            VecN result;
            for (std::size_t i = 0; i < N; i++) {
                result[i] = randomDouble(min, max);
            }

            return result;
        }

        VecN &normalize()
        {
            return *this /= length();
        }
    };

    using Vec3 = VecN<double, 3>;
    using Point3 = Vec3;
    using Color = Vec3;

    template <typename T, std::size_t N>
    std::ostream &operator<<(std::ostream &out, const VecN<T, N> &v)
    {
        for (std::size_t i = 0; i < N; i++) {
            out << v[i];
            if (i != N - 1) {
                out << " ";
            }
        }
        return out;
    }

    template <typename T, std::size_t N>
    VecN<T, N> operator+(const VecN<T, N> &u, const VecN<T, N> &v)
    {
        VecN<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.e[i] = u.e[i] + v.e[i];
        }
        return result;
    }

    template <typename T, std::size_t N>
    VecN<T, N> operator-(const VecN<T, N> &u, const VecN<T, N> &v)
    {
        VecN<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.e[i] = u.e[i] - v.e[i];
        }
        return result;
    }

    template <typename T, std::size_t N>
    VecN<T, N> operator*(const VecN<T, N> &u, const VecN<T, N> &v)
    {
        VecN<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.e[i] = u.e[i] * v.e[i];
        }
        return result;
    }

    template <typename T, std::size_t N>
    VecN<T, N> operator*(double t, const VecN<T, N> &v)
    {
        VecN<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.e[i] = t * v.e[i];
        }
        return result;
    }

    template <typename T, std::size_t N>
    VecN<T, N> operator*(const VecN<T, N> &v, double t)
    {
        return t * v;
    }

    template <typename T, std::size_t N>
    VecN<T, N> operator/(const VecN<T, N> &v, double t)
    {
        return (1 / t) * v;
    }

    template <typename T, std::size_t N>
    T dot(const VecN<T, N> &u, const VecN<T, N> &v)
    {
        T sum = 0;
        for (std::size_t i = 0; i < N; i++) {
            sum += u.e[i] * v.e[i];
        }

        return sum;
    }

    template <typename T, std::size_t N>
    VecN<T, N> cross(const VecN<T, N> &u, const VecN<T, N> &v)
    {
        static_assert(N == 3, "cross() is only valid for VecN<3>");

        VecN<T, N> result;
        result[0] = u.e[1] * v.e[2] - u.e[2] * v.e[1];
        result[1] = u.e[2] * v.e[0] - u.e[0] * v.e[2];
        result[2] = u.e[0] * v.e[1] - u.e[1] * v.e[0];

        return result;
    }

    template <typename T, std::size_t N>
    VecN<T, N> unitVector(const VecN<T, N> &v)
    {
        return v / v.length();
    }

    template <typename T, std::size_t N> VecN<T, N> randomInUnitSphere()
    {
        while (true) {
            VecN<T, N> vec = VecN<T, N>::random(-1, 1);

            if (vec.lengthSquared() >= 1) {
                continue;
            }

            return vec;
        }
    }

    template <typename T, std::size_t N> VecN<T, N> randomUnitVector()
    {
        return unitVector(randomInUnitSphere<T, N>());
    }

    template <typename T, std::size_t N>
    VecN<T, N> randomInHemisphere(const VecN<T, N> &normal)
    {
        VecN<T, N> inUnitSphere = randomInUnitSphere<T, N>();

        if (dot(inUnitSphere, normal) > 0.0) {
            return inUnitSphere;
        } else {
            return -inUnitSphere;
        }
    }

    template <typename T, std::size_t N>
    VecN<T, N> reflect(const VecN<T, N> &v, const VecN<T, N> &n)
    {
        return v - 2 * dot(v, n) * n;
    }

    template <typename T, std::size_t N>
    VecN<T, N> refract(
        const VecN<T, N> &uv, const VecN<T, N> &n, double etaiOverEtat)
    {
        double cosTheta = std::fmin(dot(-uv, n), 1.0);
        VecN<T, N> rayOutPerp = etaiOverEtat * (uv + cosTheta * n);
        VecN<T, N> rayOutParallel =
            -std::sqrt(std::fabs(1.0 - rayOutPerp.lengthSquared())) * n;
        return rayOutPerp + rayOutParallel;
    }

    template <typename T, std::size_t N> VecN<T, N> randomInUnitDisk()
    {
        while (true) {
            VecN<T, N> p =
                VecN<T, N>(randomDouble(-1, 1), randomDouble(-1, 1), 0);
            if (p.lengthSquared() >= 1) {
                continue;
            }

            return p;
        }
    }
} // namespace Raytracer::Utils

#endif /* __VEC_N_HPP__ */
