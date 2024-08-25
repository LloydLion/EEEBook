#define GUI_DRAWING_BUILTIN_PATTERN_FUNCTIONS_CPP

#include "gui/drawing/builtinPatternFunctions.h"
#include "gui/drawing/DrawingContext.h"
#include <stdint.h>

namespace builtin_pattern_functions
{
    void register_all()
    {
        auto catalog = DrawingContext::instance().pattern_catalog;
        catalog->register_pattern(PatternFunction(BUILTIN_PATTERN_NAME(chess), Chess));
        catalog->register_pattern(PatternFunction(BUILTIN_PATTERN_NAME(segments), Segments));
    }

    BUILTIN_PATTERN(chess)
    {
        auto args = parameters.read<ChessFlags::Type>(0);

        byte (&sizes)[4] = parameters.read<byte[4]>(1);
        uint16_t diviner = 0;
        for (size_t i = 0; i < 4; i++)
            diviner += sizes[i];
            
        //=====
        
        cord_t value =
            (args & ChessFlags::IgnoreA ? 0 : a.value_from_zero()) +
            (args & ChessFlags::IgnoreB ? 0 : b.value_from_zero());

        value = value % diviner;

        for (size_t i = 0; i < 4; i++)
        {
            if (value >= sizes[i])
                value -= sizes[i];
            else return i;
        }
        return 0;
    }

    BUILTIN_PATTERN(segments)
    {
        byte (&proportions)[4] = parameters.as<byte[4]>();
        uint16_t sum = 0;
        for (size_t i = 0; i < 4; i++)
            sum += proportions[i];

        cord_t size = a.size();

        cord_t size_per_unit = size / sum;

        cord_t distribution[4] = {0, 0, 0, 0};
        for (size_t i = 0; i < 4; i++)
            distribution[i] = proportions[i] * size_per_unit;

        cord_t remaining_space = size - (size_per_unit * sum);
        for (size_t i = 0; remaining_space != 0; i++)
        {
            distribution[i % size] += 1;
            remaining_space -= 1;
        }

        //=====

        cord_t value = a.value_from_zero();
        for (size_t i = 0; i < 4; i++)
        {
            if (value >= distribution[i])
                value -= distribution[i];
            else return i;
        }
        return 3;
    }
}
