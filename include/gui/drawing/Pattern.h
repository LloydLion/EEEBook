#ifndef GUI_DRAWING_PATTERN_H
#define GUI_DRAWING_PATTERN_H

#include <stdint.h>
#include "gui/coordinates.h"
#include "std/UniversalParameters.h"
#include "gui/color.h"
#include "gui/drawing/PatternFunction.h"
#include "gui/drawing/DrawingContext.h"

#define GUI_PATTERN_PALETTE_SIZE 4

struct Pattern
{
    enum DecorationFlags : uint8_t
    {
        TransposeCoordinates = 0b00000001,
        IgnoreACoordinate = 0b00000010,
        IgnoreBCoordinate = 0b00000100
    };

    enum InterpretationOptions : uint8_t
    {
        UsePathBasedCoordinateSystem = 0b00000001
    };

    PatternFunctionId function;
    UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters;
    transparent_color_t palette[GUI_PATTERN_PALETTE_SIZE];
    InterpretationOptions interpretation_options;

    struct {
        int8_t a_offset;
        int8_t b_offset;
        uint8_t a_tiling_size;
        uint8_t b_tiling_size;
        DecorationFlags flags;
    } decoration_options;


    inline Pattern()
    {
        memset(this, 0, sizeof(Pattern));
    }
    
    inline Pattern(const char *name)
    {
        memset(this, 0, sizeof(Pattern));
        function = DrawingContext::instance().pattern_catalog->find(name);
    }
    
    inline Pattern(PatternFunctionId *cache, const char *name)
    {
        memset(this, 0, sizeof(Pattern));
        if (*cache == 0)
            *cache = DrawingContext::instance().pattern_catalog->find(name);
        function = *cache;
    }

    transparent_color_t perform(CoordinateRangeValue a, CoordinateRangeValue b) const
    {
        a = a.shift(decoration_options.a_offset);
        b = b.shift(decoration_options.b_offset);

        a = a.apply_tiling(decoration_options.a_tiling_size);
        b = b.apply_tiling(decoration_options.b_tiling_size);

        if (decoration_options.flags & DecorationFlags::IgnoreACoordinate)
            a = CoordinateRangeValue(0, 0, 1);
            
        if (decoration_options.flags & DecorationFlags::IgnoreBCoordinate)
            b = CoordinateRangeValue(0, 0, 1);

        if (decoration_options.flags & DecorationFlags::TransposeCoordinates)
            std::swap(a, b);

        uint8_t index = DrawingContext::instance().pattern_catalog->get(function).function(a, b, parameters);

        return palette[index];
    }
};


#endif
