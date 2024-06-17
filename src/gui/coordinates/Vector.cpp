#include "gui/coordinates.h"

Vector::Vector(): CoordinateBase(0, 0, AxisX) { }
Vector::Vector(cord_t x, cord_t y): CoordinateBase(x, y, AxisX) { }
Vector::Vector(cord_t p, cord_t s, Axis p_axis): CoordinateBase(p, s, p_axis) { }

Vector Vector::operator+(const Vector &other) const
{
    return Vector(x() + other.x(), y() + other.y());
}

Vector Vector::operator-(const Vector &other) const
{
    return Vector(x() - other.x(), y() - other.y());
}

Vector Vector::operator-() const
{
    return Vector(-x(), -y());
}
