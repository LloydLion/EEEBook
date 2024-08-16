#include "gui/coordinates.h"
#include <stdexcept>

SignedVector::SignedVector(): CoordinateBase(0, 0, AxisX) { }
SignedVector::SignedVector(Vector vector): CoordinateBase(vector.x(), vector.y(), AxisX) { }
SignedVector::SignedVector(s_cord_t x, s_cord_t y): CoordinateBase(x, y, AxisX) { }
SignedVector::SignedVector(s_cord_t p, s_cord_t s, Axis p_axis): CoordinateBase(p, s, p_axis) { }

SignedVector SignedVector::operator+(const SignedVector &other) const
{
    return Vector(x() + other.x(), y() + other.y());
}

SignedVector SignedVector::operator-(const SignedVector &other) const
{
    return Vector(x() - other.x(), y() - other.y());
}

SignedVector SignedVector::operator-() const
{
    return Vector(-x(), -y());
}

Vector SignedVector::remove_sings()
{
    if (x() < 0 or y() < 0)
        throw std::runtime_error("Enable to cast SignedVector to Vector, some coordinate is negative");
    return Vector(x(), y());
}

Vector SignedVector::constrain()
{
    return Vector(std::max(0, x()), std::max(0, y()));
}

Vector SignedVector::absolute()
{
    return Vector(std::abs(x()), std::abs(y()));
}

bool SignedVector::is_positive()
{
    return x() >= 0 and y() >= 0;
}

SignedVector SignedVector::rotate_clockwise()
{
    return SignedVector(-y(), x());
}

SignedVector SignedVector::rotate_counter_clockwise()
{
    return SignedVector(y(), -x());
}

SignedVector SignedVector::rotate_180()
{
    return -*this;
}
