#ifndef GUI_COORDINATES_H
#define GUI_COORDINATES_H
#include <Arduino.h>

typedef uint32_t cord_t;
#define MAX_DIMENSION_SIZE 65535

#define safe_cords_sum(a, ...) __safe_cords_sum(std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value, a, __VA_ARGS__)
cord_t __safe_cords_sum(size_t va_count, cord_t a, ...);

cord_t abs_cords_subtract(cord_t a, cord_t b);

cord_t constrained_cords_subtract(cord_t a, cord_t b);


class Axis
{
private:
    uint8_t _index;

    Axis() { };
public:

    template<uint8_t index>
    static Axis create();

    Axis operator~() const;

    uint8_t index() const;
};

extern Axis AxisX;
extern Axis AxisY;

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

class Coordinates
{
public:
    enum Relationship
    {
        Smaller         = 0b100000000,
        PEqualSSmaller  = 0b010000000,
        PBiggerSSmaller = 0b001000000,
        PSmallerSEqual  = 0b000100000,
        Equal           = 0b000010000,
        PBiggerSEqual   = 0b000001000,
        PSmallerSBigger = 0b000000100,
        PEqualSBigger   = 0b000000010,
        Bigger          = 0b000000001,

        //Derivatives
        //P only
        PBigger = PBiggerSSmaller | PBiggerSEqual | Bigger,
        PEqual = PEqualSSmaller | Equal | PEqualSBigger,
        PSmaller = Smaller | PSmallerSEqual | PSmallerSBigger,

        PBiggerOrEqual = PBigger | PEqual,
        PSmallerOrEqual = PSmaller | PEqual,
        PNotEqual = PBigger | PSmaller,

        //S only
        SBigger = PSmallerSBigger | PEqualSBigger | Bigger,
        SEqual = PSmallerSEqual | Equal | PBiggerSEqual,
        SSmaller = Smaller | PEqualSSmaller | PBiggerSSmaller,

        SBiggerOrEqual = SBigger | SEqual,
        SSmallerOrEqual = SSmaller | SEqual,
        SNotEqual = SBigger | SSmaller,

        //Other
        SmallerAnyDimension = Smaller | PSmaller | SSmaller,
        SmallerAnyDimensionOrEqual = SmallerAnyDimension | Equal,
        BiggerAnyDimension = Bigger | PBigger | SBigger,
        BiggerAnyDimensionOrEqual = BiggerAnyDimension | Equal
    };
};

template<typename TInheritor>
class CoordinateBase
{
private:
    cord_t _elements[2];

public:
    
    CoordinateBase(cord_t p, cord_t s, Axis p_axis);

    static Coordinates::Relationship relate(TInheritor a, TInheritor b, Axis p_axis);
    inline static Coordinates::Relationship relate(TInheritor a, TInheritor b) { return relate(a, b, AxisX); };

    cord_t operator[](const Axis &axis) const;
    bool operator==(const TInheritor &other) const;
    bool operator!=(const TInheritor &other) const;


    //static_assert(std::is_base_of<TInheritor, CoordinateBase<TInheritor>>::value, "Inheritor must be inheritor of CoordinateBase");
};

class Vector : public CoordinateBase<Vector>
{
public:
    Vector();
    Vector(cord_t x, cord_t y);
    Vector(cord_t p, cord_t s, Axis p_axis);

    Vector operator+(const Vector &other) const;
    Vector operator-() const;
    Vector operator-(const Vector &other) const;

    inline cord_t x() const { return this->operator[](AxisX); }
    inline cord_t y() const { return this->operator[](AxisY); }
};

typedef Vector LocalVector;

class Size : public CoordinateBase<Size>
{
public:
    inline static Size max_size() { return Size(MAX_DIMENSION_SIZE, MAX_DIMENSION_SIZE); }

    Size();
    Size(cord_t width, cord_t height);
    Size(cord_t p_size, cord_t s_size, Axis p_axis);
    Size(Vector start, Vector end);

    LocalVector start_to_end() const;

    static Size intersect(Size a, Size b);
    static Size combine(Size a, Size b);

    Size operator+(const Vector &other) const;
    Size operator-(const Vector &other) const;

    inline cord_t width() const { return this->operator[](AxisX); }
    inline cord_t height() const { return this->operator[](AxisY); }
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
private:
    cord_t _up;
    cord_t _down;
    cord_t _left;
    cord_t _right;

public:
    Distance4Sides();
    Distance4Sides(cord_t up, cord_t down, cord_t left, cord_t right);
    Distance4Sides(cord_t all_directions);

    Bounds cast(Bounds original_bounds) const;
    Size expand(Size original_size) const;

    bool operator==(const Distance4Sides &other) const;
    bool operator!=(const Distance4Sides &other) const;

    inline cord_t up() const { return _up; };
    inline cord_t down() const { return _down; };
    inline cord_t left() const { return _left; };
    inline cord_t right() const { return _right; };
};

typedef Distance4Sides MarginSize;
typedef Distance4Sides PaddingSize;

#endif
