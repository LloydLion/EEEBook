#ifndef GUI_GFX_H
#define GUI_GFX_H 
#include "cordinates.h"
#include "GraphicsEngine.h"
#include "Font.h"
#include "pattern.h"

class GFX
{
private:
    Bounds _bounds;
    GraphicsEngine _engine;

    GFX(GraphicsEngine engine, Bounds bounds);
public:
    GFX(GraphicsEngine engine, Size display_size);

    void draw_rectangle(LocalBounds bounds, transparent_color_t color, cord_t thickness = 0, Pattern pattern = Pattern::get_fill_pattern()) const;
    void draw_line(Vector start_point, Vector end_point, transparent_color_t color, cord_t thickness = 0, cord_t bias = 0, Pattern pattern = Pattern::get_fill_pattern()) const;
    void draw_ellipse(Vector start_point, cord_t width, cord_t height, cord_t thickness = 0, Pattern pattern = Pattern::get_fill_pattern()) const;
    void draw_circle(Vector start_point, cord_t radius, cord_t thickness = 0, Pattern pattern = Pattern::get_fill_pattern()) const;
    void fill_screen(transparent_color_t color) const;

    void print_text(LocalVector start, cord_t width_limit, const char *text, transparent_color_t color, size_t len_limit = -1, Font font = nullptr) const;
    void print_text(LocalVector start, const char *text, transparent_color_t color, size_t len_limit = -1, Font font = nullptr) const;

    Font get_default_font() const;

    GFX slice(LocalBounds local_bounds) const;
    GFX slice(Distance4Sides distances) const;

    Size size() const;
};

#endif
