#include <limits>
#include "Common.hpp"

#ifndef __INTERVAL_HPP__
    #define __INTERVAL_HPP__

namespace Raytracer
{
    namespace Utils
    {
        class Interval {
          private:
            double _min = +std::numeric_limits<double>::infinity();
            double _max = -std::numeric_limits<double>::infinity();

          public:
            Interval() = default;
            Interval(double min, double max);
            Interval(const Interval &a, const Interval &b);
            double size() const;
            bool contains(double x) const;
            bool surrounds(double x) const;
            double clamp(double x) const;
            Interval expand(double x) const;
            static const Interval Empty;
            static const Interval Universe;
            GET_SET(double, min)
            GET_SET(double, max)
        };

        Utils::Interval operator+(const Utils::Interval &value, double offset);
        Utils::Interval operator+(double offset, const Utils::Interval &value);
    } // namespace Utils
} // namespace Raytracer

#endif /* __INTERVAL_HPP__ */
