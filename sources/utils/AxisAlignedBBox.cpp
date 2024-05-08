#include "utils/AxisAlignedBBox.hpp"
#include "utils/Interval.hpp"

/**
 * @brief Construct a new AxisAlignedBBox object.
 *
 * This function constructs a new AxisAlignedBBox object with the given x, y,
 * and z intervals. The AxisAlignedBBox is an axis-aligned bounding box that
 * represents a box in 3D space. The x, y, and z intervals represent the
 * intervals of the box along the x, y, and z axes.
 *
 * @param x The interval along the x-axis.
 * @param y The interval along the y-axis.
 * @param z The interval along the z-axis.
 *
 * @return A new AxisAlignedBBox object.
 */
Raytracer::Utils::AxisAlignedBBox::AxisAlignedBBox(
    const Interval &x, const Interval &y, const Interval &z)
    : _x(x), _y(y), _z(z)
{
    padToMinimum();
}

/**
 * @brief Construct a new AxisAlignedBBox object.
 *
 * This function constructs a new AxisAlignedBBox object with the given points.
 * The AxisAlignedBBox is an axis-aligned bounding box that represents a box in
 * 3D space. The box is defined by the two points.
 *
 * @param a The first point of the box.
 * @param b The second point of the box.
 *
 * @return A new AxisAlignedBBox object.
 */
Raytracer::Utils::AxisAlignedBBox::AxisAlignedBBox(
    const Point3 &a, const Point3 &b)
{
    _x = (a.x() <= b.x()) ? Interval(a.x(), b.x()) : Interval(b.x(), a.x());
    _y = (a.y() <= b.y()) ? Interval(a.y(), b.y()) : Interval(b.y(), a.y());
    _z = (a.z() <= b.z()) ? Interval(a.z(), b.z()) : Interval(b.z(), a.z());

    padToMinimum();
}

/**
 * @brief Construct a new AxisAlignedBBox object.
 *
 * This function constructs a new AxisAlignedBBox object by combining the given
 * AxisAlignedBBoxes. The new AxisAlignedBBox is the smallest AxisAlignedBBox
 * that contains both of the given AxisAlignedBBoxes.
 *
 * @param a The first AxisAlignedBBox.
 * @param b The second AxisAlignedBBox.
 *
 * @return A new AxisAlignedBBox object.
 */
Raytracer::Utils::AxisAlignedBBox::AxisAlignedBBox(
    const AxisAlignedBBox &a, const AxisAlignedBBox &b)
{
    _x = Interval(a.x(), b.x());
    _y = Interval(a.y(), b.y());
    _z = Interval(a.z(), b.z());
}

/**
 * @brief Get the interval along the x-axis.
 *
 * This function returns the interval along the x-axis.
 *
 * @param n The axis to get the interval for.
 *
 * @return The interval along the x-axis.
 */
const Raytracer::Utils::Interval &
Raytracer::Utils::AxisAlignedBBox::axisInterval(int n) const
{
    if (n == 1) {
        return _y;
    }
    if (n == 2) {
        return _z;
    }
    return _x;
}

/**
 * @brief Check if the ray hits the AxisAlignedBBox.
 *
 * This function checks if the ray hits the AxisAlignedBBox. The function
 * returns true if the ray hits the AxisAlignedBBox. The function returns false
 * if the ray does not hit the AxisAlignedBBox.
 *
 * @param ray The ray to check for hits.
 * @param interval The interval to check for hits.
 *
 * @return True if the ray hits the AxisAlignedBBox, false otherwise.
 */
bool Raytracer::Utils::AxisAlignedBBox::hit(
    const Core::Ray &ray, Interval interval) const
{
    const Point3 &origin = ray.origin();
    const Raytracer::Utils::Vec3 &direction = ray.direction();

    for (int axis = 0; axis < 3; axis++) {
        const Interval &ax = axisInterval(axis);
        const double invD = 1.0 / direction[axis];

        double t0 = (ax.min() - origin[axis]) * invD;
        double t1 = (ax.max() - origin[axis]) * invD;

        if (invD < 0) {
            std::swap(t0, t1);
        }

        double tmin = t0 > interval.min() ? t0 : interval.min();
        double tmax = t1 < interval.max() ? t1 : interval.max();

        if (tmax <= tmin) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Get the longest axis of the AxisAlignedBBox.
 *
 * This function returns the index of the longest axis of the AxisAlignedBBox.
 * The function returns 0 if the x-axis is the longest axis. The function
 * returns 1 if the y-axis is the longest axis. The function returns 2 if the
 * z-axis is the longest axis.
 *
 * @return The index of the longest axis of the AxisAlignedBBox.
 */
int Raytracer::Utils::AxisAlignedBBox::longestAxis() const
{
    double x = _x.size();
    double y = _y.size();
    double z = _z.size();

    if (x > y) {
        return x > z ? 0 : 2;
    } else {
        return y > z ? 1 : 2;
    }
}

/**
 * @brief Pad the AxisAlignedBBox to the minimum size.
 *
 * This function pads the AxisAlignedBBox to the minimum size. The function
 * expands the intervals of the AxisAlignedBBox to the minimum size if the
 * intervals are smaller than the minimum size.
 *
 * @return void
 */
void Raytracer::Utils::AxisAlignedBBox::padToMinimum()
{
    double delta = 0.0001;

    if (_x.size() < delta) {
        _x = _x.expand(delta);
    }
    if (_y.size() < delta) {
        _y = _y.expand(delta);
    }
    if (_z.size() < delta) {
        _z = _z.expand(delta);
    }
}

/**
 * @brief Empty AxisAlignedBBox.
 *
 * This constant represents an empty AxisAlignedBBox.
 */
const Raytracer::Utils::AxisAlignedBBox
    Raytracer::Utils::AxisAlignedBBox::Empty =
        AxisAlignedBBox(Interval::Empty, Interval::Empty, Interval::Empty);

/**
 * @brief Universe AxisAlignedBBox.
 *
 * This constant represents the universe AxisAlignedBBox.
 */
const Raytracer::Utils::AxisAlignedBBox
    Raytracer::Utils::AxisAlignedBBox::Universe = AxisAlignedBBox(
        Interval::Universe, Interval::Universe, Interval::Universe);

/**
 * @brief Operator to add a Vec3 to an AxisAlignedBBox.
 *
 * This operator adds a Vec3 to an AxisAlignedBBox. The operator returns a new
 * AxisAlignedBBox with the Vec3 added to the AxisAlignedBBox.
 *
 * @param value The AxisAlignedBBox to add the Vec3 to.
 * @param offset The Vec3 to add to the AxisAlignedBBox.
 *
 * @return The AxisAlignedBBox with the Vec3 added to it.
 */
Raytracer::Utils::AxisAlignedBBox Raytracer::Utils::operator+(
    const Raytracer::Utils::AxisAlignedBBox &value,
    Raytracer::Utils::Vec3 offset)
{
    return Raytracer::Utils::AxisAlignedBBox(value.x() + offset.x(),
        value.y() + offset.y(), value.z() + offset.z());
}

/**
 * @brief Operator to add an AxisAlignedBBox to a Vec3.
 *
 * This operator adds an AxisAlignedBBox to a Vec3. The operator returns a new
 * AxisAlignedBBox with the Vec3 added to the AxisAlignedBBox.
 *
 * @param offset The Vec3 to add to the AxisAlignedBBox.
 * @param value The AxisAlignedBBox to add the Vec3 to.
 *
 * @return The AxisAlignedBBox with the Vec3 added to it.
 */
Raytracer::Utils::AxisAlignedBBox Raytracer::Utils::operator+(
    Raytracer::Utils::Vec3 offset,
    const Raytracer::Utils::AxisAlignedBBox &value)
{
    return value + offset;
}
