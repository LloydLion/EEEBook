#ifndef GUI_GRAPHICS_ENGINE_H
#define GUI_GRAPHICS_ENGINE_H
#include "cordinates.h"
#include "gui/Font.h"

class GraphicsEngine_;
typedef GraphicsEngine_ *GraphicsEngine;

class GraphicsEngine_
{
public:
    virtual void draw_rectangle(Bounds bounds) = 0;
    virtual void print_text(Point start_point, cord_t width_limit, const char *text, Font font) = 0;

    virtual Font get_default_font() = 0;
    virtual FontEngine get_font_engine() = 0;

    virtual void push() = 0;
};

#endif
