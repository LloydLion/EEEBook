#ifndef GUI_DRAWING_SCREEN_H
#define GUI_DRAWING_SCREEN_H

#include "gui/color.h"
#include "gui/coordinates.h"

class Screen_;
typedef Screen_ *Screen;

class Screen_
{
public:
    virtual void draw_pixel(Vector position, color_t color) = 0;
    virtual void draw_vertical_line(Vector position, cord_t size, color_t color) = 0;
    virtual void draw_horizontal_line(Vector position, cord_t size, color_t color) = 0;

    virtual Size full_viewport_size() = 0;

    virtual void initialize() = 0;
    virtual void begin() = 0;
    virtual void send() = 0;
    virtual void clear() = 0;
};

#endif
