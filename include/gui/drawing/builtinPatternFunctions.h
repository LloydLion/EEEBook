#ifndef GUI_DRAWING_BUILTIN_PATTERN_FUNCTIONS_H
#define GUI_DRAWING_BUILTIN_PATTERN_FUNCTIONS_H

#include "gui/drawing/Pattern.h"

#define BUILTIN_PATTERN_NAME(X) __builtin_pattern_function__##X
#define BUILTIN_PATTERN(X) uint8_t BUILTIN_PATTERN_NAME(X)(CoordinateRangeValue a, \
    CoordinateRangeValue b, UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters, PatternState<GUI_PATTERN_STATE_SIZE> *cache)

namespace builtin_pattern_functions
{
    void register_all();


    constexpr char const Chess[] = "chess";

    struct ChessFlags
    {
        enum Type : uint8_t
        {
            UseAB = 0b00,
            IgnoreA = 0b10,
            IgnoreB = 0b01
        };
    };

    BUILTIN_PATTERN(chess);

    
    constexpr char const Segments[] = "segments";

    BUILTIN_PATTERN(segments);    
    //BUILTIN_PATTERN(bitmap);
}

#ifndef GUI_DRAWING_BUILTIN_PATTERN_FUNCTIONS_CPP
#undef BUILTIN_PATTERN
#undef BUILTIN_PATTERN_NAME
#endif
#endif
