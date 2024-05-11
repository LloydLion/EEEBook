#ifndef GUI_GRAPHICS_ENGINE_H
#define GUI_GRAPHICS_ENGINE_H
#include "gui/cordinates.h"
#include "gui/Font.h"
#include "gui/color.h"
#include "gui/UpdateRule.h"
#include "gui/DrawSettings.h"
#include "Bitmap.h"
#include "pattern.h"

class GraphicsEngine_;
typedef GraphicsEngine_ *GraphicsEngine;

class GraphicsEngine_
{
public:
    virtual void draw_rectangle(Bounds bounds, color_t color, cord_t thickness, Pattern pattern) = 0;
    virtual void draw_line(Vector start_point, Vector end_point, color_t color, cord_t thickness, cord_t bias, Pattern pattern) = 0;
    virtual void draw_ellipse(Bounds bounds, color_t color, cord_t thickness, Pattern pattern) = 0;
    virtual void draw_bitmap(Vector star_point, Bitmap bitmap, color_t color) = 0;

    virtual void print_text(Vector start_point, cord_t width_limit, const char *text, size_t len_limit, color_t color, Font font) = 0;

    virtual Font get_default_font() = 0;
    virtual FontEngine get_font_engine() = 0;

    virtual void push(DrawSettings settings) = 0;
};

#endif
