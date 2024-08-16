#include "gui/coordinates.h"

Bounds::Bounds(): start(), size() { }
Bounds::Bounds(Vector start, Size size): start(start), size(size) { }
Bounds::Bounds(Vector start, Vector end): start(Vector(std::min(start.x(), start.x()), std::min(end.y(), end.y()))), size(start, end) { }
    
bool Bounds::is_inside_abs(Vector absolute_vector) const
{
    return
        absolute_vector.x() >= start.x() &&
        absolute_vector.x() < size.width() &&
        absolute_vector.y() >= start.y() &&
        absolute_vector.y() < size.height();
}

bool Bounds::is_inside_local(LocalVector local_vector) const
{
    return local_vector.x() < size.width() and local_vector.y() < size.height();
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
    return start == other.start and size == other.size;
}

bool Bounds::operator!=(const Bounds &other) const
{
    return !this->operator==(other);
}

Bounds Bounds::intersect(Bounds a, Bounds b)
{
    Vector new_start = Vector(std::max(a.start.x(), b.start.x()), std::max(a.start.y(), b.start.y()));
    Vector new_end = Vector(std::min(a.end().x(), b.end().x()), std::min(a.end().y(), b.end().y()));
    return Bounds(new_start, new_end);
}

Bounds Bounds::combine(Bounds a, Bounds b)
{
    Vector new_start = Vector(std::min(a.start.x(), b.start.x()), std::min(a.start.y(), b.start.y()));
    Vector new_end = Vector(std::max(a.end().x(), b.end().x()), std::max(a.end().y(), b.end().y()));
    return Bounds(new_start, new_end);
}
