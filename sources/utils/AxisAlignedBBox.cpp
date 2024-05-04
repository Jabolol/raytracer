#include "utils/AxisAlignedBBox.hpp"
#include "utils/Interval.hpp"

Raytracer::Utils::AxisAlignedBBox::AxisAlignedBBox(
    const Interval &x, const Interval &y, const Interval &z)
    : _x(x), _y(y), _z(z)
{
    padToMinimum();
}

Raytracer::Utils::AxisAlignedBBox::AxisAlignedBBox(
    const Point3 &a, const Point3 &b)
{
    _x = (a.x() <= b.x()) ? Interval(a.x(), b.x()) : Interval(b.x(), a.x());
    _y = (a.y() <= b.y()) ? Interval(a.y(), b.y()) : Interval(b.y(), a.y());
    _z = (a.z() <= b.z()) ? Interval(a.z(), b.z()) : Interval(b.z(), a.z());

    padToMinimum();
}

Raytracer::Utils::AxisAlignedBBox::AxisAlignedBBox(
    const AxisAlignedBBox &a, const AxisAlignedBBox &b)
{
    _x = Interval(a.x(), b.x());
    _y = Interval(a.y(), b.y());
    _z = Interval(a.z(), b.z());
}

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

const Raytracer::Utils::AxisAlignedBBox
    Raytracer::Utils::AxisAlignedBBox::Empty =
        AxisAlignedBBox(Interval::Empty, Interval::Empty, Interval::Empty);

const Raytracer::Utils::AxisAlignedBBox
    Raytracer::Utils::AxisAlignedBBox::Universe = AxisAlignedBBox(
        Interval::Universe, Interval::Universe, Interval::Universe);

Raytracer::Utils::AxisAlignedBBox Raytracer::Utils::operator+(
    const Raytracer::Utils::AxisAlignedBBox &value,
    Raytracer::Utils::Vec3 offset)
{
    return Raytracer::Utils::AxisAlignedBBox(value.x() + offset.x(),
        value.y() + offset.y(), value.z() + offset.z());
}

Raytracer::Utils::AxisAlignedBBox Raytracer::Utils::operator+(
    Raytracer::Utils::Vec3 offset,
    const Raytracer::Utils::AxisAlignedBBox &value)
{
    return value + offset;
}
