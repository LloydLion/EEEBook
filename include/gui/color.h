#ifndef GUI_COLOR_H
#define GUI_COLOR_H
#include <Arduino.h>

enum color_t : uint8_t
{
    Black = 0,
    White = 1,
};

uint16_t color_to_rgb565(color_t color);
color_t get_opposite_color(color_t color);

#endif
