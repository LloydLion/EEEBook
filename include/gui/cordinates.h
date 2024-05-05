#ifndef GUI_COORDINATES_H
#define GUI_COORDINATES_H

typedef unsigned int cord_t;

class Vector
{
public:
    cord_t x;
    cord_t y;

    Vector();
    Vector(cord_t x, cord_t y);

    Vector operator+(const Vector &other) const;
    Vector operator-() const;
    Vector operator-(const Vector &other) const;
};

typedef Vector LocalVector;

class Size
{
public:
    cord_t width;
    cord_t height;

    Size();
    Size(cord_t width, cord_t height);
    Size(Vector start, Vector end);

    LocalVector start_to_end() const;

    Size operator+(const Vector &other) const;
    Size operator-(const Vector &other) const;
};

class Bounds;
typedef Bounds LocalBounds;

class Bounds
{
public:
    Vector start;
    Size size;

    Bounds();
    Bounds(Vector start, Size size);
    Bounds(Vector start, Vector end);
    Bounds(cord_t x, cord_t y, cord_t w, cord_t h);

    bool is_inside_abs(Vector absolute_vector) const;
    bool is_inside_local(LocalVector local_vector) const;
    Bounds slice(LocalVector local_vector, Size size) const;
    Bounds cast(LocalBounds local_bounds) const;
    Vector cast(LocalVector local_vector) const;
    Vector end() const;
};

class Distance4Sides
{
public:
    cord_t up;
    cord_t down;
    cord_t left;
    cord_t right;

    Distance4Sides();
    Distance4Sides(cord_t up, cord_t down, cord_t left, cord_t right);
    Distance4Sides(cord_t all_directions);

    Bounds cast(Bounds original_bounds) const;
    Size expand(Size original_size) const;
};

typedef Distance4Sides MarginSize;
typedef Distance4Sides PaddingSize;

#endif
