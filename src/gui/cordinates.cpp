#include "gui/cordinates.h"

Point create_point(cord_t x, cord_t y)
{
    Point result;
    result.x = x;
    result.y = y;
    return result;
}

Size create_size(cord_t width, cord_t height)
{
    Size result;
    result.width = width;
    result.height = height;
    return result;
}

Bounds create_bounds(Point start_point, Size size)
{
    Bounds result;
    result.start_point = start_point;
    result.size = size;
    return result;
}

Bounds create_bounds(Point start_point, Point end_point)
{
    Size size;
    size.width = end_point.x - start_point.x;
    size.height = end_point.y - start_point.y;
    return create_bounds(start_point,size);    
}

bool point_is_inside(Point point, Bounds bound)
{
    return point.x >= bound.start_point.x and
        point.x <= bound.start_point.x + bound.size.width and
        point.y >= bound.start_point.y and
        point.y <= bound.start_point.y + bound.size.height;
}

Bounds cast(Bounds outer_bounds, Bounds sub_bounds)
{
    Bounds result;
    result.start_point.x = outer_bounds.start_point.x + sub_bounds.start_point.x;
    result.start_point.y = outer_bounds.start_point.y + sub_bounds.start_point.y;
    result.size = sub_bounds.size;
    return result;

}