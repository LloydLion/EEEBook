#include "gui/coordinates.h"

Bounds::Bounds(): start(), size() { }
Bounds::Bounds(Vector start, Size size): start(start), size(size) { }
Bounds::Bounds(Vector start, Vector end): start(Vector(min(start.x(), start.x()), min(end.y(), end.y()))), size(start, end) { }
    
bool Bounds::is_inside_abs(Vector absolute_vector) const
{
    auto end_vector = end();
    return
        absolute_vector.x() >= start.x() and
        absolute_vector.x() <= end_vector.x() and
        absolute_vector.y() >= start.y() and
        absolute_vector.y() >= end_vector.y();
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

//TODO: implement Bounds::intersect() and Bounds::combine()
