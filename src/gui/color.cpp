#include "gui/color.h"

uint16_t color_to_rgb565(color_t color)
{
    switch (color)
    {
    case color_t::White:
        return 0xFFFF;

    case color_t::Black:
        return 0x0000;

#ifdef IS_RBW_DISPLAY
    case color_t::Red:
        return 0xF800;
#endif

    default:
        return 0x0000;
    }
}


transparent_color_t transparent_color()
{
    transparent_color_t tcolor;
    tcolor.is_transparent = true;
    tcolor.color = color_t::Black;
    return tcolor;
}

transparent_color_t::transparent_color_t(const color_t& other)
{
    is_transparent = false;
    color = other;
}

transparent_color_t::transparent_color_t()
{
    is_transparent = true;
    color = color_t::Black;
}
