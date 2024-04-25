#ifndef GUI_COORDINATES_H
#define GUI_COORDINATES_H

typedef unsigned int cord_t;

struct Point
{
    cord_t x;
    cord_t y;
};

struct Size
{
    cord_t width;
    cord_t height;
};

struct Bounds
{
    Point start_point;
    Size size;
};

Point create_point(cord_t x, cord_t y);
Size create_size(cord_t width, cord_t height);
Bounds create_bounds(Point start_point, Size size);
Bounds create_bounds(Point start_point, Point end_point);
bool point_is_inside(Point point, Bounds bound);
Bounds cast(Bounds outer_bounds, Bounds sub_bounds);

#endif
