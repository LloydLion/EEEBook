#ifndef GUI_COORDINATES_H
#define GUI_COORDINATES_H
#include <Arduino.h>

typedef uint32_t cord_t;
#define MAX_DIMENSION_SIZE 65535

// First bit represents vertical position (0=up, 1=down), second - horizontal postion (0=left, 1=right)
enum class Corner
{
    LeftUp = 0b00,
    LeftDown = 0b01,
    RightUp = 0b10,
    RightDown = 0b11
};

// If first bit is set, Side represents vertical side (up/down), else horizontal side (left/right)
enum class Side
{
    Up = 0b00,
    Down = 0b10,
    Left = 0b01,
    Right = 0b11,
};

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

    bool operator==(const Vector &other) const;
    bool operator!=(const Vector &other) const;
};

typedef Vector LocalVector;

class Size
{
public:
    enum Relationship : uint16_t
    {
        Smaller         = 0b100000000,
        EqualWSmallerH  = 0b010000000,
        WiderSmallerH   = 0b001000000,
        SmallerWEqualH  = 0b000100000,
        SmallerWHigher  = 0b000010000,
        Equal           = 0b000001000,
        WiderEqualH     = 0b000000100,
        EqualWHigher    = 0b000000010,
        Bigger          = 0b000000001,

        //Derivativeso
        Wider = WiderSmallerH | WiderEqualH | Bigger,
        EqualWide = EqualWSmallerH | Equal | EqualWHigher,
        LessWide = Smaller | SmallerWEqualH | SmallerWHigher,

        WiderOrEqual = Wider | EqualWide,
        LessWideOrEqual = LessWide | EqualWide,
        NotEqualWide = Wider | LessWide,

        Higher = SmallerWHigher | EqualWHigher | Bigger,
        EqualHigh = SmallerWEqualH | Equal | WiderEqualH,
        LessHigh = Smaller | EqualWSmallerH | WiderSmallerH,

        HigherOrEqual = Higher | EqualHigh,
        LessHighOrEqual = LessHigh | EqualHigh,
        NotEqualHigh = Higher | LessHigh,

        SmallerAnyDimension = Smaller | SmallerWEqualH | EqualWSmallerH,
        EqualOrSmaller = SmallerAnyDimension | Equal,

        BiggerOrEqual = Equal | WiderEqualH | EqualWHigher | Bigger
    };

    static Size max_size() { return Size(MAX_DIMENSION_SIZE, MAX_DIMENSION_SIZE); }

    cord_t width;
    cord_t height;

    Size();
    Size(cord_t width, cord_t height);
    Size(Vector start, Vector end);

    LocalVector start_to_end() const;

    static Size intersect(Size a, Size b);
    static Size combine(Size a, Size b);

    Size operator+(const Vector &other) const;
    Size operator-(const Vector &other) const;

    bool operator>(const Size &other) const;
    bool operator<(const Size &other) const;

    bool operator==(const Size &other) const;
    bool operator!=(const Size &other) const;

    Relationship operator%(const Size &other) const;
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
    
    static Bounds intersect(Bounds a, Bounds b);
    static Bounds combine(Bounds a, Bounds b);

    bool operator==(const Bounds &other) const;
    bool operator!=(const Bounds &other) const;
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
    Size safe_expand(Size original_size) const;

    bool operator==(const Distance4Sides &other) const;
    bool operator!=(const Distance4Sides &other) const;
};

typedef Distance4Sides MarginSize;
typedef Distance4Sides PaddingSize;

#endif
