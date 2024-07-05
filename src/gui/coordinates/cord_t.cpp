#include "gui/coordinates.h"

cord_t __safe_cords_sum(size_t va_count, cord_t a, ...)
{
    va_list args;
    va_start(args, a);

    cord_t sum = a;

    for (size_t i = 0; i < va_count; i++)
    {
        cord_t value = va_arg(args, cord_t);
        sum += value;

        if (sum < value) //Overflow
        {
            va_end(args);
            return MAX_DIMENSION_SIZE;
        }
    }

    va_end(args);
    return sum;
}

cord_t abs_cords_subtract(cord_t a, cord_t b)
{
    if (a > b)
        return a - b;
    else return b - a;
}

cord_t constrained_cords_subtract(cord_t a, cord_t b)
{
    if (b > a)
        return 0;
    else return a - b;
}
