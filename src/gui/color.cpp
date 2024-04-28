#include "gui/color.h"

uint16_t color_to_rgb565(color_t color)
{
    switch (color)
    {
    case color_t::White:
        return 0xFFFF;

    case color_t::Black:
        return 0x0000;

    default:
        return 0x0000;
    }
}

color_t get_opposite_color(color_t color)
{
    switch (color)
    {
    case color_t::White:
        return color_t::Black;

    case color_t::Black:
        return color_t::White;

    default:
        return color_t::Black;
    }
}