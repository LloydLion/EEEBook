#ifndef GUI_DRAWING_PATTERN_FUNCTION_H
#define GUI_DRAWING_PATTERN_FUNCTION_H

#include <stdint.h>
#include "std/UniversalParameters.h"
#include "gui/coordinates.h"
#include "platform/pointer.h"

#define GUI_PATTERN_PARAMETERS_SIZE 8
#define GUI_PATTERN_STATE_SIZE 64

struct CoordinateRangeValue
{
    s_cord_t start;
    s_cord_t end;
    s_cord_t value;

    inline CoordinateRangeValue(s_cord_t value, s_cord_t start, s_cord_t end):
        value(value), start(start), end(end)
    { }

    inline cord_t size() const { return std::abs(end - start); }
    inline s_cord_t direction() const { return end >= start ? 1 : -1; }
    inline cord_t value_from_zero() { return (cord_t)(value - start); };
    inline s_cord_t inclusive_end() const { return end - 1; }

    inline CoordinateRangeValue shift(s_cord_t offset) const
        { return CoordinateRangeValue(value + offset, start + offset, end + offset); }
    inline CoordinateRangeValue apply_tiling(cord_t size) const
    {
        if (size == 0 || size == 1) return *this;
        
        auto from_zero = shift(-start);
        s_cord_t new_end = from_zero.end / size + (from_zero.end % size == 0 ? 0 : 1);
        auto tiled = CoordinateRangeValue(from_zero.value / size, 0, new_end);
        return tiled.shift(start / size);
    }

    inline operator s_cord_t() const { return value; }
};

template<size_t size>
struct PatternState
{
    uint8_t state;
    UniversalParameters<size> store;
};


struct PatternFunction
{
    using FunctionType = uint8_t (*)(
        CoordinateRangeValue a, CoordinateRangeValue b,
        UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters,
        PatternState<GUI_PATTERN_STATE_SIZE> *cache
    );

    FunctionType function;
    const char *name;

    inline PatternFunction(FunctionType function, const char* name):
        name(name), function(function) { }
};

typedef uint16_t PatternFunctionId;

#endif
