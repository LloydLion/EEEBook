#ifndef GUI_COLOR_H
#define GUI_COLOR_H
#include <Arduino.h>
#include "config.h"

enum color_t : uint8_t
{
    Black = 0,
    White = 1,
#ifdef IS_RBW_DISPLAY
    Red = 2,
#endif
};

struct transparent_color_t
{
    bool is_transparent;
    color_t color;

    transparent_color_t(const color_t& other);
    transparent_color_t();
};

transparent_color_t transparent_color();

uint16_t color_to_rgb565(color_t color);

#endif
