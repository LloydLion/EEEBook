#include "gui/coordinates.h"

Distance4Sides::Distance4Sides(): _up(), _down(), _left(), _right()
{ }

Distance4Sides::Distance4Sides(cord_t up, cord_t down, cord_t left, cord_t right):
    _up(up), _down(down), _left(left), _right(right)
{ }

Distance4Sides::Distance4Sides(cord_t all_directions):
    _up(all_directions), _down(all_directions), _left(all_directions), _right(all_directions)
{

}


Bounds Distance4Sides::cast(Bounds original_bounds) const
{
    return Bounds(original_bounds.start + Vector(left(), up()), original_bounds.size - Vector(left() + right(), up() + down()));
}

Size Distance4Sides::expand(Size original_size) const
{
    return Size(safe_cords_sum(original_size.width(), left(), right()), safe_cords_sum(original_size.height(), up(), down()));
}

bool Distance4Sides::operator==(const Distance4Sides &other) const
{
    return _left == other._left and _right == other._right and _up == other._up and _down == other._down;
}

bool Distance4Sides::operator!=(const Distance4Sides &other) const
{
    return _left != other._left or _right != other._right or _up != other._up or _down != other._down;
}
