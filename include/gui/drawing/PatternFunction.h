#ifndef GUI_DRAWING_PATTERN_FUNCTION_H
#define GUI_DRAWING_PATTERN_FUNCTION_H

#include <stdint.h>
#include "std/UniversalParameters.h"
#include "gui/coordinates.h"
#include "platform/pointer.h"

#define GUI_PATTERN_PARAMETERS_SIZE 16

struct PatternFunction
{
    using FunctionType = uint8_t (*)(
        s_cord_t a, s_cord_t b,
        cord_t a_size, cord_t b_size,
        UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters
    );

    FunctionType function;
    const char *name;

    inline PatternFunction(FunctionType function, const char* name):
        name(name), function(function) { }
};

typedef uint16_t PatternFunctionId;

#endif
