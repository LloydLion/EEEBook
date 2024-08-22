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
        s_cord_t a_offset;
        s_cord_t b_offset;
        DecorationFlags flags;
    } decoration_options;


    transparent_color_t perform(CoordinateRangeValue a, CoordinateRangeValue b) const
    {
        a = a.shift(decoration_options.a_offset);
        b = b.shift(decoration_options.b_offset);

        if (decoration_options.flags & DecorationFlags::TransposeCoordinates)
            std::swap(a, b);

        uint8_t index = DrawingContext::instance().pattern_catalog->get(function).function(a, b, parameters);

        return palette[index];
    }
};


#endif
