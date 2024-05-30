#include "gui/cordinates.h"
#include <Arduino.h>

#define DEFAULT_NOT_EQUAL_IMPL { return !(other == *this); }

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

bool Vector::operator==(const Vector &other) const
{
    return x == other.x && y == other.y;
}

bool Vector::operator!=(const Vector &other) const DEFAULT_NOT_EQUAL_IMPL

Size::Size(): width(), height() { }
Size::Size(cord_t width, cord_t height): width(width), height(height) { }
Size::Size(Vector start, Vector end): width(end.x - start.x + 1), height(end.y - start.y + 1) { }

LocalVector Size::start_to_end() const
{
    return LocalVector(width - 1, height - 1);
}

bool Size::is_null() const
{
    return width == 0 and height == 0;
}

Size Size::intersect(Size a, Size b)
{
    return Size(min(a.width, b.width), min(a.height, b.height));
}

Size Size::combine(Size a, Size b)
{
    return Size(max(a.width, b.width), max(a.height, b.height));
}

Size Size::operator+(const Vector &other) const
{
    return Size(width + other.x, height + other.y);
}

Size Size::operator-(const Vector &other) const
{
    return Size(width - other.x, height - other.y);
}

bool Size::operator>(const Size &other) const
{
    return operator%(other) & Relationship::Bigger;
}

bool Size::operator<(const Size &other) const
{
    return operator%(other) & Relationship::Smaller;
}

bool Size::operator==(const Size &other) const
{
    return operator%(other) & Relationship::Equal;
}

bool Size::operator!=(const Size &other) const DEFAULT_NOT_EQUAL_IMPL

Size::Relationship Size::operator%(const Size &other) const
{
    if (other.width > width && other.height > height)
        return Relationship::Bigger;
    else if (other.width > width && other.height == height)
        return Relationship::WiderEqualH;
    else if (other.width > width && other.height < height)
        return Relationship::WiderSmallerH;

    else if (other.width == width && other.height > height)
        return Relationship::EqualWHigher;
    else if (other.width == width && other.height == height)
        return Relationship::Equal;
    else if (other.width == width && other.height < height)
        return Relationship::EqualWSmallerH;

    else if (other.width < width && other.height > height)
        return Relationship::SmallerWHigher;
    else if (other.width < width && other.height == height)
        return Relationship::SmallerWEqualH;
    else //if (other.width < width && other.height < height)
        return Relationship::Smaller;
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

Bounds Bounds::cast(LocalBounds local_bounds) const
{
    return Bounds(cast(local_bounds.start), local_bounds.size);
}

Bounds Bounds::slice(LocalVector local_vector, Size size) const
{
    return Bounds(cast(local_vector), size);
}

bool Bounds::operator==(const Bounds &other) const
{
    return start == other.start && size == other.size;
}

bool Bounds::operator!=(const Bounds &other) const DEFAULT_NOT_EQUAL_IMPL

Distance4Sides::Distance4Sides():
    up(), down(), left(), right() { }
Distance4Sides::Distance4Sides(cord_t up, cord_t down, cord_t left, cord_t right):
    up(up), down(down), left(left), right(right) { }
Distance4Sides::Distance4Sides(cord_t all_directions):
    up(all_directions), down(all_directions), left(all_directions), right(all_directions) { }

Bounds Distance4Sides::cast(Bounds original_bounds) const
{
    return Bounds(original_bounds.start + Vector(left, up), original_bounds.size - Vector(left + right, up + down));
}

Size Distance4Sides::expand(Size original_size) const
{
    return Size(original_size.width + left + right, original_size.height + up + down);
}

bool Distance4Sides::operator==(const Distance4Sides &other) const
{
    return up == other.up && down == other.down && left == other.left && right == other.right;
}

bool Distance4Sides::operator!=(const Distance4Sides &other) const DEFAULT_NOT_EQUAL_IMPL
