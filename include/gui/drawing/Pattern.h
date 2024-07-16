#ifndef GUI_DRAWING_PATTERN_H
#define GUI_DRAWING_PATTERN_H

#include <stdint.h>
#include "gui/coordinates.h"
#include "std/UniversalParameters.h"
#include "gui/color.h"
#include "gui/drawing/PatternFunction.h"
#include "gui/drawing/PatternCatalog.h"

#define GUI_PATTERN_PALETTE_SIZE 4

struct Pattern
{
    enum Flags
    {
        TransposeCoordinates = 0b00000001,
    };

    PatternFunctionId function;
    UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters;
    transparent_color_t palette[GUI_PATTERN_PALETTE_SIZE];

    struct {
        s_cord_t a_offset;
        s_cord_t b_offset;
        Flags flags;
    } decoration_options;

    transparent_color_t perform(PatternCatalog *catalog, s_cord_t a, s_cord_t b, cord_t a_size, cord_t b_size)
    {
        a += decoration_options.a_offset;
        a_size += decoration_options.a_offset;
        b += decoration_options.b_offset;
        b_size += decoration_options.b_offset;

        if (decoration_options.flags & Flags::TransposeCoordinates)
        {
            std::swap(a, b);
            std::swap(a_size, b_size);
        }

        uint8_t index = catalog->get(function).function(a, b, a_size, b_size, parameters);

        return palette[index];
    }
};


#endif
