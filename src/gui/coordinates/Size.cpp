#include "gui/coordinates.h"

Size::Size(): CoordinateBase(0, 0, AxisX)
{ }

Size::Size(cord_t width, cord_t height): CoordinateBase(width, height, AxisX)
{ }

Size::Size(cord_t p_size, cord_t s_size, Axis p_axis): CoordinateBase(p_size, s_size, p_axis)
{ }

Size::Size(Vector start, Vector end): Size(
    abs_cords_subtract(end.x(), start.x()) + 1,
    abs_cords_subtract(end.y(), start.y()) + 1
)
{ }


LocalVector Size::start_to_end() const
{
    return LocalVector(width() - 1, height() - 1);
}

Size Size::intersect(Size a, Size b)
{
    return Size(min(a.width(), b.width()), min(a.height(), b.height()));
}

Size Size::combine(Size a, Size b)
{
    return Size(max(a.width(), b.width()), max(a.height(), b.height()));
}

Size Size::operator+(const Vector &other) const
{
    return Size(width() + other.x(), height() + other.y());
}

Size Size::operator-(const Vector &other) const
{
    return Size(width() - other.x(), height() - other.y());
}
