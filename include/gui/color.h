#ifndef GUI_COLOR_H
#define GUI_COLOR_H
#include <stdint.h>
#include <stdexcept>
#include "config.h"
#include "std/byte.h"

typedef uint8_t color_t;
struct transparent_color_t;

template<typename TColor> struct ColorDepth;

#if COLOR_MODEL == BW_COLOR_MODEL
template<> struct ColorDepth<color_t> { static constexpr size_t depth = 1; };
template<> struct ColorDepth<transparent_color_t> { static constexpr size_t depth = 2; };
class ColorMap
{
public:
    static constexpr color_t Black = 0;
    static constexpr color_t White = 1;
};
#elif COLOR_MODEL == BWR_COLOR_MODEL
template<> struct ColorDepth<color_t> { static constexpr size_t depth = 2; };
template<> struct ColorDepth<transparent_color_t> { static constexpr size_t depth = 2; };
class ColorMap
{
public:
    static constexpr color_t Black = 0;
    static constexpr color_t White = 1;
    static constexpr color_t Red = 2;
};
#endif

uint16_t color_to_rgb565(color_t color);

struct transparent_color_t
{
    static constexpr color_t reserved_value = ~(~0u << ColorDepth<transparent_color_t>::depth);
    color_t raw_color;

    inline transparent_color_t(const color_t& other): raw_color(other) { }
    inline transparent_color_t(): raw_color(reserved_value) { }

    inline bool is_transparent() const { return raw_color == reserved_value; }
    inline color_t color() const
    {
        if (is_transparent())
            throw std::runtime_error("Enable to use transparent color as non transparent");
        return raw_color;
    }

    inline bool operator==(const transparent_color_t &other) const
    {
        return raw_color == other.raw_color;
    }

    inline operator uint8_t() const { return raw_color; }
};


template<typename TColor>
class ColorArrayBase
{
protected:
    static constexpr size_t _depth = ColorDepth<TColor>::depth;

private:
    static constexpr uint16_t _mask = ~(~0u << _depth);

    byte *_array;
    size_t _array_size;

public:
    TColor get(size_t index) const;
    void set(size_t index, TColor color);

    inline ColorArrayBase(byte *array, size_t array_size): _array(array), _array_size(array_size) { }
};

template<typename TColor, size_t size>
class ColorArray : public ColorArrayBase<TColor>
{
private:
    static constexpr size_t _colors_pure_size = size * ColorArrayBase<TColor>::_depth;
    static constexpr size_t _array_size = _colors_pure_size % 8 == 0 ? _colors_pure_size : (_colors_pure_size + 8 - (_colors_pure_size % 8));

    byte _array[_array_size];

public:
    inline ColorArray(): _array(), ColorArrayBase<TColor>(&_array, _array_size) { }
    inline ColorArray(std::initializer_list<TColor> items): _array(), ColorArrayBase<TColor>(&_array, _array_size)
    {
        for (size_t i = 0; i < items.size(); i++)
            set(i, *(items.begin() + i));
    }
};

#endif
