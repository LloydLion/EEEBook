#ifndef GUI_DRAWING_PATTERN_BAG_H
#define GUI_DRAWING_PATTERN_BAG_H

#include "gui/drawing/Pattern.h"
#include <cstring>

struct PatternBag
{
    Pattern pattern;
    PatternState<GUI_PATTERN_STATE_SIZE> state;


    inline PatternBag(): pattern()
    {

    }

    inline PatternBag(Pattern pattern): pattern(pattern)
    {
        memset(&state, 0, sizeof(PatternState<GUI_PATTERN_STATE_SIZE>));
    }

    inline transparent_color_t perform(CoordinateRangeValue a, CoordinateRangeValue b)
    {
        return pattern.perform(a, b, &state);
    }
};

#endif
