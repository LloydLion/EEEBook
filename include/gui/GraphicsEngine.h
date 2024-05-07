#ifndef GUI_GRAPHICS_ENGINE_H
#define GUI_GRAPHICS_ENGINE_H
#include "cordinates.h"
#include "gui/Font.h"
#include "color.h"

class GraphicsEngine_;
typedef GraphicsEngine_ *GraphicsEngine;

class GraphicsEngine_
{
public:
    virtual void draw_rectangle(Bounds bounds, color_t color, cord_t thickness) = 0;
    virtual void print_text(Vector start_point, cord_t width_limit, const char *text, size_t len_limit, color_t color, Font font) = 0;

    virtual Font get_default_font() = 0;
    virtual FontEngine get_font_engine() = 0;

    virtual void push() = 0;
};

#endif
