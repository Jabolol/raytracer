#include "utils/Interval.hpp"
#include <algorithm>

Raytracer::Utils::Interval::Interval(double min, double max)
    : _min(min), _max(max)
{
}

Raytracer::Utils::Interval::Interval(const Interval &a, const Interval &b)
{
    _min = a.min() <= b.min() ? a.min() : b.min();
    _max = a.max() >= b.max() ? a.max() : b.max();
}

double Raytracer::Utils::Interval::size() const
{
    return _max - _min;
}

bool Raytracer::Utils::Interval::contains(double x) const
{
    return _min <= x && x <= _max;
}

bool Raytracer::Utils::Interval::surrounds(double x) const
{
    return _min < x && x < _max;
}

double Raytracer::Utils::Interval::clamp(double x) const
{
    return std::min(std::max(x, _min), _max);
}

Raytracer::Utils::Interval Raytracer::Utils::Interval::expand(double x) const
{
    double padding = x / 2;

    return Interval(_min - padding, _max + padding);
}

const Raytracer::Utils::Interval Raytracer::Utils::Interval::Empty =
    Interval(+std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity());
const Raytracer::Utils::Interval Raytracer::Utils::Interval::Universe =
    Interval(-std::numeric_limits<double>::infinity(),
        +std::numeric_limits<double>::infinity());

Raytracer::Utils::Interval Raytracer::Utils::operator+(
    const Raytracer::Utils::Interval &value, double offset)
{
    return Raytracer::Utils::Interval(
        value.min() + offset, value.max() + offset);
}

Raytracer::Utils::Interval Raytracer::Utils::operator+(
    double offset, const Raytracer::Utils::Interval &value)
{
    return value + offset;
}
