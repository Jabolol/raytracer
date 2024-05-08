#include "utils/Interval.hpp"
#include <algorithm>

/**
 * @brief Construct a new Interval object.
 *
 * This function constructs a new Interval object with the given minimum and
 * maximum values.
 *
 * @param min The minimum value.
 * @param max The maximum value.
 *
 * @return A new Interval object.
 */
Raytracer::Utils::Interval::Interval(double min, double max)
    : _min(min), _max(max)
{
}

/**
 * @brief Construct a new Interval object.
 *
 * This function constructs a new Interval object by combining the given
 * Intervals. The new Interval is the smallest Interval that contains both of
 * the given Intervals.
 *
 * @param a The first Interval.
 * @param b The second Interval.
 *
 * @return A new Interval object.
 */
Raytracer::Utils::Interval::Interval(const Interval &a, const Interval &b)
{
    _min = a.min() <= b.min() ? a.min() : b.min();
    _max = a.max() >= b.max() ? a.max() : b.max();
}

/**
 * @brief Get the minimum value of the interval.
 *
 * This function returns the minimum value of the interval.
 *
 * @return The minimum value of the interval.
 */
double Raytracer::Utils::Interval::size() const
{
    return _max - _min;
}

/**
 * @brief Check if the interval contains the given value.
 *
 * This function checks if the interval contains the given value. The function
 * returns true if the interval contains the value, false otherwise.
 *
 * @param x The value to check.
 *
 * @return True if the interval contains the value, false otherwise.
 */
bool Raytracer::Utils::Interval::contains(double x) const
{
    return _min <= x && x <= _max;
}

/**
 * @brief Check if the interval surrounds the given value.
 *
 * This function checks if the interval surrounds the given value. The function
 * returns true if the interval surrounds the value, false otherwise.
 *
 * @param x The value to check.
 *
 * @return True if the interval surrounds the value, false otherwise.
 */
bool Raytracer::Utils::Interval::surrounds(double x) const
{
    return _min < x && x < _max;
}

/**
 * @brief Clamp the value to the interval.
 *
 * This function clamps the value to the interval. The function returns the
 * clamped value.
 *
 * @param x The value to clamp.
 *
 * @return The clamped value.
 */
double Raytracer::Utils::Interval::clamp(double x) const
{
    return std::min(std::max(x, _min), _max);
}

/**
 * @brief Expand the interval by the given value.
 *
 * This function expands the interval by the given value. The function returns
 * the expanded interval.
 *
 * @param x The value to expand the interval by.
 *
 * @return The expanded interval.
 */
Raytracer::Utils::Interval Raytracer::Utils::Interval::expand(double x) const
{
    double padding = x / 2;

    return Interval(_min - padding, _max + padding);
}

/**
 * @brief A constant empty interval.
 *
 * This constant represents an empty interval.
 */
const Raytracer::Utils::Interval Raytracer::Utils::Interval::Empty =
    Interval(+std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity());

/**
 * @brief A constant universe interval.
 *
 * This constant represents the universe interval.
 */
const Raytracer::Utils::Interval Raytracer::Utils::Interval::Universe =
    Interval(-std::numeric_limits<double>::infinity(),
        +std::numeric_limits<double>::infinity());

/**
 * @brief Operator to add a double to an Interval.
 *
 * This operator adds a double to an Interval. The operator returns a new
 * Interval with the double added to the Interval.
 *
 * @param value The Interval to add the double to.
 * @param offset The double to add to the Interval.
 *
 * @return The Interval with the double added to it.
 */
Raytracer::Utils::Interval Raytracer::Utils::operator+(
    const Raytracer::Utils::Interval &value, double offset)
{
    return Raytracer::Utils::Interval(
        value.min() + offset, value.max() + offset);
}

/**
 * @brief Operator to add a double to an Interval.
 *
 * This operator adds a double to an Interval. The operator returns a new
 * Interval with the double added to the Interval.
 *
 * @param offset The double to add to the Interval.
 * @param value The Interval to add the double to.
 *
 * @return The Interval with the double added to it.
 */
Raytracer::Utils::Interval Raytracer::Utils::operator+(
    double offset, const Raytracer::Utils::Interval &value)
{
    return value + offset;
}
