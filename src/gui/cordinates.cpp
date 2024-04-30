#include "gui/cordinates.h"

Vector::Vector(): x(), y() { }
Vector::Vector(cord_t x, cord_t y): x(x), y(y) { }

Vector Vector::operator+(const Vector &other) const
{
    return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector &other) const
{
    return Vector(x - other.x, y - other.y);
}

Vector Vector::operator-() const
{
    return Vector(-x, -y);
}

Size::Size(): width(), height() { }
Size::Size(cord_t width, cord_t height): width(width), height(height) { }
Size::Size(Vector start, Vector end): width(end.x - start.x), height(end.y - start.y) { }

LocalVector Size::start_to_end() const
{
    return LocalVector(width, height);
}

Size Size::operator+(const Vector &other) const
{
    return Size(width + other.x, height + other.y);
}

Size Size::operator-(const Vector &other) const
{
    return Size(width - other.x, height - other.y);
}

Bounds::Bounds(): start(), size() { }
Bounds::Bounds(Vector start, Size size): start(start), size(size) { }
Bounds::Bounds(Vector start, Vector end): start(start), size(start, end) { }
Bounds::Bounds(cord_t x, cord_t y, cord_t w, cord_t h): start(x, y), size(w, h) { }
    
bool Bounds::is_inside_abs(Vector absolute_vector) const
{
    auto endv = end();
    return
        absolute_vector.x >= start.x and
        absolute_vector.x <= endv.x and
        absolute_vector.y >= start.y and
        absolute_vector.y >= endv.y;
}

bool Bounds::is_inside_local(LocalVector local_vector) const
{
    return local_vector.x <= size.width and local_vector.y <= size.height;
}

Vector Bounds::cast(LocalVector local_vector) const
{
    return start + local_vector;
}

Vector Bounds::end() const
{
    return start + size.start_to_end();    
}

Distance4Sides::Distance4Sides(cord_t up, cord_t down, cord_t left, cord_t right):
    up(up), down(down), left(left), right(right) { }

Bounds Distance4Sides::cast(Bounds original_bounds) const
{
    return Bounds(original_bounds.start + Vector(left, up), original_bounds.size - Vector(right, down));
}

Bounds Bounds::cast(LocalBounds local_bounds) const
{
    return Bounds(cast(local_bounds.start), local_bounds.size);
}

Bounds Bounds::slice(LocalVector local_vector, Size size) const
{
    return Bounds(cast(local_vector), size);
}
